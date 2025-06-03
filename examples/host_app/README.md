# Host Application Using FVM

This example demonstrates how to build an application for the device, in this case the device being the Flipper Virtual Machine. This example prints a string.

#### First Step

```sh
cd $HOME/flipper/
```
or wherever your flipper installation lives

#### Next Step

To ensure all the python builds effectively you ust be inside a python environment using pyelftools

```sh
source .venv/bin/activate
pip3 install pyelftools 
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
```

#### Next Step

must use gcc here because the inline attributes fail to build with clang

```sh
make module MODULE=fvm_test 
make call_fvm

```
#### Next Step

```sh
DYLD_LIBRARY_PATH=$HOME/flipper/.build/libflipper \
$HOME/flipper/.build/fvm/fvm \
$HOME/flipper/.build/fvm_test.so fvm_test
```

#### Next Step

In a new Terminal Window (you can press command + t)
```sh
cd /src
```
Use this for the module to be on any available port

```sh
DYLD_LIBRARY_PATH=$HOME/flipper/.build/libflipper ./call_fvm fvm_test
```
FVM looks for any open UDP port and attaches so if you want to use a specific port then do the following instead :

```sh
DYLD_LIBRARY_PATH=$HOME/flipper/.build/libflipper ./call_fvm fvm_test xxxxx
```
Where xxxxx is the UDP port FVM printed it is listening on from the other terminal, for example 62845

