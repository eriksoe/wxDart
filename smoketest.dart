import 'package:wxdart/wxdart.dart';

void main() {
    print("Hello!");
    var a = quux();
    print("a => ${a}");
    var b = quux1(123);
    print("b => $b");
    var ires = InitializeWx();

    var fr = new Frame("Hello, World!").Show(true);
    print("Init => ${ires}");
    var mres = MainLoop();
    print("Loop => ${mres}");
}
