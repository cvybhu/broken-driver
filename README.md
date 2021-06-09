# broken-driver
Broken scylla cpp driver

This repository contains a minimal example where scylla cpp driver breaks

# To run the example:
### Build the example inside a docker container:
```bash
# Requires being in the docker group or sudo
./build.sh
```

### Start local scylla instance
```bash
# Requires being in the docker group or sudo
./scylla.sh
```
### Wait for scylla to start
Should say something like `Listening on 173.17.0.3`

### Run the example:
```bash
# Requires being in the docker group or sudo
./run.sh
```

### Wait for it to break
Normally it should finish after a few seconds

Address sanitizer prints an error
# Configuration

### Use datastax cassandra driver (doesn't break)
In `Dockerfile` replace lines which install scylla driver with lines which install Datastax cassandra driver

### Make a release build
In `Dockerfile` replace last line with the one containing release options

The program finishes but it does not print `DONE` - something is wrong.


# Errors
```
ASAN:DEADLYSIGNAL
=================================================================
==1==ERROR: AddressSanitizer: SEGV on unknown address 0x000000000000 (pc 0x7f474077b50d bp 0x7f473aef7660 sp 0x7f473aef7600 T5)
==1==The signal is caused by a WRITE memory access.
==1==Hint: address points to the zero page.
    #0 0x7f474077b50c in uv_timer_stop (/usr/lib/x86_64-linux-gnu/libuv.so.1+0x1850c)
    #1 0x7f474076ea94 in uv_close (/usr/lib/x86_64-linux-gnu/libuv.so.1+0xba94)
    #2 0x7f4742bddbde in datastax::internal::core::Timer::stop() (/usr/lib/x86_64-linux-gnu/libscylla-cpp-driver.so.2+0x1d8bde)
    #3 0x7f4742b98636 in datastax::internal::core::RequestHandler::set_response(datastax::internal::SharedRefPtr<datastax::internal::core::Host> const&, datastax::internal::SharedRefPtr<datastax::internal::core::Response> const&) (/usr/lib/x86_64-linux-gnu/libscylla-cpp-driver.so.2+0x193636)
    #4 0x7f4742b9a2ce in datastax::internal::core::RequestExecution::on_result_response(datastax::internal::core::Connection*, datastax::internal::core::ResponseMessage*) (/usr/lib/x86_64-linux-gnu/libscylla-cpp-driver.so.2+0x1952ce)
    #5 0x7f4742b9a8bc in datastax::internal::core::RequestExecution::on_set(datastax::internal::core::ResponseMessage*) (/usr/lib/x86_64-linux-gnu/libscylla-cpp-driver.so.2+0x1958bc)
    #6 0x7f4742af1e27 in datastax::internal::core::Connection::on_read(char const*, unsigned long) (/usr/lib/x86_64-linux-gnu/libscylla-cpp-driver.so.2+0xece27)
    #7 0x7f4742af21d7 in datastax::internal::core::ConnectionHandler::on_read(datastax::internal::core::Socket*, long, uv_buf_t const*) (/usr/lib/x86_64-linux-gnu/libscylla-cpp-driver.so.2+0xed1d7)
    #8 0x7f47407785ce  (/usr/lib/x86_64-linux-gnu/libuv.so.1+0x155ce)
    #9 0x7f474077933b  (/usr/lib/x86_64-linux-gnu/libuv.so.1+0x1633b)
    #10 0x7f474077e33f in uv__io_poll (/usr/lib/x86_64-linux-gnu/libuv.so.1+0x1b33f)
    #11 0x7f474076ecc7 in uv_run (/usr/lib/x86_64-linux-gnu/libuv.so.1+0xbcc7)
    #12 0x7f4742b4be34 in datastax::internal::core::EventLoop::handle_run() (/usr/lib/x86_64-linux-gnu/libscylla-cpp-driver.so.2+0x146e34)
    #13 0x7f4740d2e6da in start_thread (/lib/x86_64-linux-gnu/libpthread.so.0+0x76da)
    #14 0x7f474147371e in __clone (/lib/x86_64-linux-gnu/libc.so.6+0x12171e)

AddressSanitizer can not provide additional info.
SUMMARY: AddressSanitizer: SEGV (/usr/lib/x86_64-linux-gnu/libuv.so.1+0x1850c) in uv_timer_stop
Thread T5 created by T1 here:
    #0 0x7f4742edbd2f in __interceptor_pthread_create (/usr/lib/x86_64-linux-gnu/libasan.so.4+0x37d2f)
    #1 0x7f474077acc0 in uv_thread_create (/usr/lib/x86_64-linux-gnu/libuv.so.1+0x17cc0)

Thread T1 created by T0 here:
    #0 0x7f4742edbd2f in __interceptor_pthread_create (/usr/lib/x86_64-linux-gnu/libasan.so.4+0x37d2f)
    #1 0x7f474077acc0 in uv_thread_create (/usr/lib/x86_64-linux-gnu/libuv.so.1+0x17cc0)

==1==ABORTING
```

Or sometimes this happens:
```
==1==ERROR: AddressSanitizer: heap-use-after-free on address 0x60c00002f5c0 at pc 0x7f2986dc9601 bp 0x7f297edfac40 sp 0x7f297edfa3e8
READ of size 128 at 0x60c00002f5c0 thread T5
    #0 0x7f2986dc9600  (/usr/lib/x86_64-linux-gnu/libasan.so.4+0x50600)
    #1 0x7f2986dc9bb4 in writev (/usr/lib/x86_64-linux-gnu/libasan.so.4+0x50bb4)
    #2 0x7f298464da7a  (/usr/lib/x86_64-linux-gnu/libuv.so.1+0x15a7a)
    #3 0x7f298464e357  (/usr/lib/x86_64-linux-gnu/libuv.so.1+0x16357)
    #4 0x7f2984643c9a in uv_run (/usr/lib/x86_64-linux-gnu/libuv.so.1+0xbc9a)
    #5 0x7f2986a20e34 in datastax::internal::core::EventLoop::handle_run() (/usr/lib/x86_64-linux-gnu/libscylla-cpp-driver.so.2+0x146e34)
    #6 0x7f2984c036da in start_thread (/lib/x86_64-linux-gnu/libpthread.so.0+0x76da)
    #7 0x7f298534871e in __clone (/lib/x86_64-linux-gnu/libc.so.6+0x12171e)

0x60c00002f5c0 is located 0 bytes inside of 128-byte region [0x60c00002f5c0,0x60c00002f640)
freed by thread T3 here:
    #0 0x7f2986e577a8 in __interceptor_free (/usr/lib/x86_64-linux-gnu/libasan.so.4+0xde7a8)
    #1 0x7f2984641699 in uv__free (/usr/lib/x86_64-linux-gnu/libuv.so.1+0x9699)

previously allocated by thread T3 here:
    #0 0x7f2986e57b40 in __interceptor_malloc (/usr/lib/x86_64-linux-gnu/libasan.so.4+0xdeb40)
    #1 0x7f298464eee0 in uv_write2 (/usr/lib/x86_64-linux-gnu/libuv.so.1+0x16ee0)

Thread T5 created by T1 here:
    #0 0x7f2986db0d2f in __interceptor_pthread_create (/usr/lib/x86_64-linux-gnu/libasan.so.4+0x37d2f)
    #1 0x7f298464fcc0 in uv_thread_create (/usr/lib/x86_64-linux-gnu/libuv.so.1+0x17cc0)

Thread T1 created by T0 here:
    #0 0x7f2986db0d2f in __interceptor_pthread_create (/usr/lib/x86_64-linux-gnu/libasan.so.4+0x37d2f)
    #1 0x7f298464fcc0 in uv_thread_create (/usr/lib/x86_64-linux-gnu/libuv.so.1+0x17cc0)

Thread T3 created by T1 here:
    #0 0x7f2986db0d2f in __interceptor_pthread_create (/usr/lib/x86_64-linux-gnu/libasan.so.4+0x37d2f)
    #1 0x7f298464fcc0 in uv_thread_create (/usr/lib/x86_64-linux-gnu/libuv.so.1+0x17cc0)

SUMMARY: AddressSanitizer: heap-use-after-free (/usr/lib/x86_64-linux-gnu/libasan.so.4+0x50600) 
Shadow bytes around the buggy address:
  0x0c187fffde60: 00 00 00 00 00 00 00 00 fa fa fa fa fa fa fa fa
  0x0c187fffde70: fd fd fd fd fd fd fd fd fd fd fd fd fd fd fd fd
  0x0c187fffde80: fa fa fa fa fa fa fa fa fd fd fd fd fd fd fd fd
  0x0c187fffde90: fd fd fd fd fd fd fd fd fa fa fa fa fa fa fa fa
  0x0c187fffdea0: fd fd fd fd fd fd fd fd fd fd fd fd fd fd fd fd
=>0x0c187fffdeb0: fa fa fa fa fa fa fa fa[fd]fd fd fd fd fd fd fd
  0x0c187fffdec0: fd fd fd fd fd fd fd fd fa fa fa fa fa fa fa fa
  0x0c187fffded0: fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa
  0x0c187fffdee0: fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa
  0x0c187fffdef0: fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa
  0x0c187fffdf00: fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa fa
Shadow byte legend (one shadow byte represents 8 application bytes):
  Addressable:           00
  Partially addressable: 01 02 03 04 05 06 07 
  Heap left redzone:       fa
  Freed heap region:       fd
  Stack left redzone:      f1
  Stack mid redzone:       f2
  Stack right redzone:     f3
  Stack after return:      f5
  Stack use after scope:   f8
  Global redzone:          f9
  Global init order:       f6
  Poisoned by user:        f7
  Container overflow:      fc
  Array cookie:            ac
  Intra object redzone:    bb
  ASan internal:           fe
  Left alloca redzone:     ca
  Right alloca redzone:    cb
==1==ABORTING
```