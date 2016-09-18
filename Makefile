CPPFLAGS=-Wall -ggdb `wx-config --cppflags`
LDFLAGS=-Wall -ggdb  `wx-config --libs`

smoketest: lib/libwxdart.so lib/wxdart.dart
	dart smoketest.dart

lib/libwxdart.so: lib/wxdart.o
	echo "A: $<"
	echo "B: $+"
	gcc ${LDFLAGS} -shared -m64 -Wl,-soname,$@.so -o "$@" $< ${LDFLAGS}

%.o: %.cpp
	echo "A: $<"
	echo "B: $+"
	g++ ${CPPFLAGS} -fPIC -m64 -I/usr/lib/dart/include -DDART_SHARED_LIB -c $< -o "$@"

clean:
	rm -fv lib/*.o lib/*.so

