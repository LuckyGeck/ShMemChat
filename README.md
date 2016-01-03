### Схема работы

#### Message Queue over ShMem

Есть область shared memory, в которой лежат сообщения. Представление в памяти следущее:
 * Переменная типа `size_t` - сколько байт занимают все сообщения
 * zero-terminated string сообщения 1
 * zero-terminated string сообщения 2
 * ...
 * zero-terminated string сообщения k

Работа с данной областью происходит через класс `NShQ::TShQ` из файла `shq.h`.
Поддерживаются две операции:
 * `void AppendMsg(const char* msg)` - добавить сообщение в очередь.
 * `size_t ProcessAndClean(std::function<void(const char*)> fn)` - вызвать `fn` для каждого сообщения в очереди и очистить её. Возвращает количество сообщений, которые `fn` получил на вход.

Обе операции захватывают `mutex` (бинарный `semaphore`) для обеспечения единоличного доступа к памяти.

#### Оповещение о приходе нового сообщения

Каждый клиент, при попытке послать новое сообщение, сначала добавляет его в очередь. После этого увеличивает на 1 значение `semaphore` для оповещений.

Сервер же пытается сделать уменьшение значения `semaphore` на 1.

Пока новых сообщений нету, он находится в состоянии блокировки и ожидания на семафоре.

Как только приходит хотя бы одно сообщение, операция сервера проходит успешно. Далее он обрабатывает все сообщения в очереди и очищает ее. На последней стадии обработки сообщений сервер уменьшает семафор на `k - 1`, где `k` - количество обработанных сообщений, после чего пытается уменьшить значение семафора еще на 1, возвращаясь тем самым в исходное состояние ожидания.