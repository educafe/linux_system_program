void **retval 은 retval의 값이 pointer, 즉 주소를 가리키고, 해당 주소의 값 또한 주소를 가리키는 것이며 
최종 주소를 찾아가야 실제 변수의 값을 얻을 수 있다는 의미.

pthread_exit (void *retval)로 반환하는 값을
pthread_join(pthread_t thread, void **retval) 의 형식으로 받는다는 것은
retval 변수가 main에서 void * retval로 선언되어 있고, retval 값은 주소라는 의미이며, 따라서 pthread_exit(void *retval)에서
보내주는 값이 주소이므로, 실제 pthread_exit에서 반환하는 값을 읽기 위해서는 **를 사용해야 한다.

한편,
thread 함수에서 void * retval 로 선언한 후 해당 변수에 값을 저장한 후 pthread_exit (void *retval)로 보낼 경우 
함수가 종료되면 해당 변수가 stack에서 없어지므로 malloc을 사용하여 heap 영역에 저장해 놓은 후 pthread_join에서
읽어야 한다.
