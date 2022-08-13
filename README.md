# bob_slub_debugging

## slubtest
### module build & load
```bash
cd slubtest/module
make
sudo insmod slubtest.ko
```
### test program (user)

```bash
cd slubtest
gcc -o test test.c
```

## kprobe
### module build & load
```bash
cd kprobe/[dir]
make
sudo insmod [name].ko
```

### check message

```bash
sudo dmesg
```
