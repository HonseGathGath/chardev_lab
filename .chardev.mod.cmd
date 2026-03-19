savedcmd_chardev.mod := printf '%s\n'   chardev.o | awk '!x[$$0]++ { print("./"$$0) }' > chardev.mod
