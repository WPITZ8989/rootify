rootify
=======

rootify is an easy-to-use privilege escalation tool. It's a peculiar combination of su and sudo.

```
$ rootify -h
rootify gives root privileges for the users who are listed in /etc/rootify.conf (one username per line)
Usage: ./rootify [options | program]
Options: -h, --help : Print this help text.
If program is specified then it will be executed with root privileges.
If no program is specified then a root shell will be opened.
rootify 0.2.3 is released under the GPLv3, hosted at https://github.com/mondalaci/rootify
```

But you have to install it first. You can directly copy either the rootify.i386 or the rootify.amd64 binary to your path or build it by typing "make".

After that do something like this as root:

```
# echo joe >> /etc/rootify.conf
```

From this point forward it's easy for Joe to become root:

```
joe$ rootify
#
```

It's also easy for him to run any programs as root:

```
joe# rootify ls /root
...
```
