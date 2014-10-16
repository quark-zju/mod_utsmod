utsmod
======

Provide a sysctl interface to modify read-only UTS fields (ostype, osrelease, version).

<img src="data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAT8AAAAdAgMAAACh24OxAAAACXBIWXMAAAsTAAALEwEAmpwYAAAAB3RJTUUH3goQCxY6KIgbRQAAAAlQTFRFAAAA7ykp////dSTLRAAAAAFiS0dEAmYLfGQAAAGcSURBVEjH7ZQxihtBEEWfGxY+kysfFIk6hX0DGeT7fDoq5hTFRkOf0kGPdwVOvLJCVdrFo37V/w2venpd6imYayx7tLIZxfkZQFuSbN79v8BfP2+nH7fuVVIZYBhvFUsF0L68gtvb2/V6TVtSGuAMTpFpA/qy2gnsSOp3wMgcZvT9MWBP5g4PySlnGtCDE1qfwDmhp+QHgN9up++n2A7bTGCkIjOAtr+y+Q/lp7R9HG/YQJuG6RUSrQJiPvfC2IAvzaBhZoICAtHKKfIT6Hu+7RSSDwgkzbx79hhkmHYymAvSBH4qad56Tc807E0gm25iqUhkstfoFg5WQ5/OB5OISjVxWcopYOCSJJ1B2JlQDdKQ6a1Wk6mGhfE6IM251YDRhMhUm7noK2DsZZckWLHzMuNR2x6ZTtk4JSwcNqZ8xMoptPeUjqD5AIoJNHYKtIKNM52a0V6xsO15owO47XOHfwEPyQdQMv4AcmzMwu7Ytjkfh9MErveSW7mV5v//xzbDDojMmLbpNXpFq+gEDnC/s02vddlDitQr1U+o39NPy3DmGU/VAAAAAElFTkSuQmCC"/>

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
