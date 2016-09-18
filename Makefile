CPPFLAGS=-Wall -ggdb `wx-config --cppflags`
LDFLAGS=-Wall -ggdb  `wx-config --libs`

smoketest: libwxdart.so wxdart.dart
	dart smoketest.dart

libwxdart.so: wxdart.o
	gcc ${LDFLAGS} -shared -m64 -Wl,-soname,$@.so -o "$@" $< ${LDFLAGS}
%.o: %.cpp
	g++ ${CPPFLAGS} -fPIC -m64 -I/usr/lib/dart/include -DDART_SHARED_LIB -c wxdart.cpp

clean:
	rm -fv *.o *.so

