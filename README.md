utsmod
======

Provide a sysctl interface to modify read-only UTS fields (ostype, osrelease, version).

![uname -a: Firefox ZION 32.0-amd64 Mozilla/5.0 x86_64 GNU/Linux](http://quark-zju.github.io/mod_utsmod/public/uname-demo.png)

Example
-------

```bash
# ./build.sh 
...
# insmod utsmod.ko
# sysctl -a | grep utsmod
utsmod.osrelease = 3.12-0.bpo.1-amd64
utsmod.ostype = Linux
utsmod.version = #1 SMP Debian 3.12.9-1~bpo70+1 (2014-02-07)
# uname -a
Linux debian 3.12-0.bpo.1-amd64 #1 SMP Debian 3.12.9-1~bpo70+1 (2014-02-07) x86_64 GNU/Linux

# sysctl -w utsmod.ostype=Firefox
sysctl: setting key "utsmod.ostype": Operation not permitted

# unshare --uts bash
# sysctl -w utsmod.ostype=Firefox
# sysctl -w utsmod.osrelease='1.0.0-foo-amd64'
# sysctl -w utsmod.version='Mozilla/5.0 (X11; Linux x86_64; rv:32.0) Gecko/20100101'
# hostname ZION
# uname -a
Firefox ZION 1.0.0-foo-amd64 Mozilla/5.0 (X11; Linux x86_64; rv:32.0) Gecko/20100101 x86_64 GNU/Linux

# exit
# uname -a
Linux debian 3.12-0.bpo.1-amd64 #1 SMP Debian 3.12.9-1~bpo70+1 (2014-02-07) x86_64 GNU/Linux
```

Notes
-----
- Tested in Linux 3.2, 3.12, 3.16
- Root UTS namespace is read-only
