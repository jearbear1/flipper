# Host Application Using FVM

This example demonstrates how to build an application for the device, in this case the device being the Flipper Virtual Machine. This example prints a string and returns an int.
to `IO_1`.

#### First Step

```sh
cd $HOME/flipper/
```
or wherever your flipper installation lives

#### Next Step

To ensure all the python builds effectively you ust be inside a python environment using pyelftools

```sh
source .venv/bin/activate
pip 3 install pyelftools 
```

#### Next Step

```sh
Make install-utils PREFIX=/opt/homebrew/bin
```
then to ensure proper build 

```sh
Which fvm
```
you should see

```sh
/opt/homebrew/bin/fvm 
```

#### Next Step

```sh
cd $HOME/flipper/
make clean 
```
#### Next Step

```sh
make libflipper
make utils
```

#### Next Step

```sh
cd examples/host_app
make clean
make
```
#### Next Step

```sh
DYLD_LIBRARY_PATH=$HOME/flipper/.build/libflipper \
$HOME/flipper/.build/fvm/fvm \
$HOME/flipper/.build/fvm_test.so fvm_
```

#### Next Step

In a New Terminal Window

```sh
cd examples/host_app
```
#### Next Step

must use gcc here because the inline attributes fail to build with clang

```sh
gcc-12 \
  -I../../platforms/posix/include \
  -I../../lib \
  -I../../.build/include \
  -L../../.build/libflipper \
  -lflipper \
  src/client_calls_fvm.c -o src/call_fvm
```
#### Next Step

```sh
cd /example/host_app/src
```

#### Next Step

```sh
DYLD_LIBRARY_PATH=$HOME/flipper/.build/libflipper ./call_fvm 
```
FVM looks for any open UDP port and attaches so if you want to use a specific port then do :

```sh
DYLD_LIBRARY_PATH=$HOME/flipper/.build/libflipper ./call_fvm xxxxx
```
instead
