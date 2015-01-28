# Project: Mines-Perfect-Linux

CC   = gcc #clang-3.5
# WINDRES = windres.exe
OBJ  = app.o api.o dialogs.o ../core/board.o ../core/eqs.o ../core/logbook.o ../core/options.o ../core/perfana.o ../core/utils.o ../core/vargroup.o ../gui/bevelctrl.o ../gui/bitmapctrl.o ../gui/boardctrl.o ../gui/buttonctrl.o ../gui/ctrl.o ../gui/gamectrl.o ../gui/lcdctrl.o ../gui/smileyctrl.o
LIBS = -lm -lstdc++ -L/usr/lib/x86_64-linux-gnu -pthread -Wl,-Bsymbolic-functions -Wl,-z,relro  -L/usr/lib/x86_64-linux-gnu   -lwx_gtk2u_richtext-2.8 -lwx_gtk2u_aui-2.8 -lwx_gtk2u_xrc-2.8 -lwx_gtk2u_qa-2.8 -lwx_gtk2u_html-2.8 -lwx_gtk2u_adv-2.8 -lwx_gtk2u_core-2.8 -lwx_baseu_xml-2.8 -lwx_baseu_net-2.8 -lwx_baseu-2.8

INCS =  -I"/usr/include/wx-2.8/" -I"/usr/lib/x86_64-linux-gnu/wx/include/gtk2-unicode-release-2.8"
BIN  = "mineperf.elf"
CFLAGS = $(INCS) -D_FILE_OFFSET_BITS=64 -D_LARGE_FILES -D__WXGTK__ -DLINUX -DwxUSE_UNICODE=1 -fno-exceptions -fstrict-aliasing -Wall -fexceptions -s -pthread -Wno-write-strings -D_VARIANT_=4 # I (James) cannot find any info about _VARIANT_ except that it is used when saving/loading log files. The Windows binary from SourceForge (version 1.4.0) uses _VARIANT_=3; since I'm making a fork, I decided to up it to 4.

all : $(OBJ)
	$(CC) $(OBJ) -o $(BIN) $(RES) $(LIBS) $(CFLAGS)

app.o : app.cpp
	$(CC) -c app.cpp $(CFLAGS)

api.o : api.cpp
	$(CC) -c api.cpp $(CFLAGS)

dialogs.o : dialogs.cpp
	$(CC) -c dialogs.cpp $(CFLAGS)

../core/board.o : ../core/board.cpp
	$(CC) -c ../core/board.cpp $(CFLAGS) -o ../core/board.o

../core/eqs.o : ../core/eqs.cpp
	$(CC) -c ../core/eqs.cpp $(CFLAGS) -o ../core/eqs.o

../core/logbook.o : ../core/logbook.cpp
	$(CC) -c ../core/logbook.cpp $(CFLAGS) -o ../core/logbook.o

../core/options.o : ../core/options.cpp
	$(CC) -c ../core/options.cpp $(CFLAGS) -o ../core/options.o

../core/perfana.o : ../core/perfana.cpp
	$(CC) -c ../core/perfana.cpp $(CFLAGS) -o ../core/perfana.o

../core/utils.o : ../core/utils.cpp
	$(CC) -c ../core/utils.cpp $(CFLAGS) -o ../core/utils.o

../core/vargroup.o : ../core/vargroup.cpp
	$(CC) -c ../core/vargroup.cpp $(CFLAGS) -o ../core/vargroup.o

../gui/bevelctrl.o : ../gui/bevelctrl.cpp
	$(CC) -c ../gui/bevelctrl.cpp $(CFLAGS) -o ../gui/bevelctrl.o

../gui/bitmapctrl.o : ../gui/bitmapctrl.cpp
	$(CC) -c ../gui/bitmapctrl.cpp $(CFLAGS) -o ../gui/bitmapctrl.o

../gui/boardctrl.o : ../gui/boardctrl.cpp
	$(CC) -c ../gui/boardctrl.cpp $(CFLAGS) -o ../gui/boardctrl.o

../gui/buttonctrl.o : ../gui/buttonctrl.cpp
	$(CC) -c ../gui/buttonctrl.cpp $(CFLAGS) -o ../gui/buttonctrl.o

../gui/ctrl.o : ../gui/ctrl.cpp
	$(CC) -c ../gui/ctrl.cpp $(CFLAGS) -o ../gui/ctrl.o

../gui/gamectrl.o : ../gui/gamectrl.cpp
	$(CC) -c ../gui/gamectrl.cpp $(CFLAGS) -o ../gui/gamectrl.o

../gui/lcdctrl.o : ../gui/lcdctrl.cpp
	$(CC) -c ../gui/lcdctrl.cpp $(CFLAGS) -o ../gui/lcdctrl.o

../gui/smileyctrl.o : ../gui/smileyctrl.cpp
	$(CC) -c ../gui/smileyctrl.cpp $(CFLAGS) -o ../gui/smileyctrl.o

clean :
	rm -f ./*.o
	rm -f ../core/*.o
	rm -f ../gui/*.o
