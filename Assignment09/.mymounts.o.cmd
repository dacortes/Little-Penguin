savedcmd_mymounts.o := ld -m elf_x86_64 -z noexecstack --no-warn-rwx-segments   -r -o mymounts.o @mymounts.mod  ; /sources/linux-next-next-20251114/tools/objtool/objtool --hacks=jump_label --hacks=noinstr --hacks=skylake --ibt --orc --retpoline --rethunk --static-call --uaccess --prefix=16  --link  --module mymounts.o

mymounts.o: $(wildcard /sources/linux-next-next-20251114/tools/objtool/objtool)
