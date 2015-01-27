# Project: Mines-Perfect-Linux

CC   = gcc
# WINDRES = windres.exe
OBJ  = "app.o" "api.o" 
LIBS =  -L/usr/lib/x86_64-linux-gnu -pthread -Wl,-Bsymbolic-functions -Wl,-z,relro  -L/usr/lib/x86_64-linux-gnu   -lwx_gtk2u_richtext-2.8 -lwx_gtk2u_aui-2.8 -lwx_gtk2u_xrc-2.8 -lwx_gtk2u_qa-2.8 -lwx_gtk2u_html-2.8 -lwx_gtk2u_adv-2.8 -lwx_gtk2u_core-2.8 -lwx_baseu_xml-2.8 -lwx_baseu_net-2.8 -lwx_baseu-2.8 -lcomctl32 -lgcc -lstdc++ -luuid -lxpm
INCS =  -I"/usr/include/wx-2.8/" -I"/usr/lib/x86_64-linux-gnu/wx/include/gtk2-unicode-release-2.8"
BIN  = "mineperf.elf"
CFLAGS = $(INCS) -D_FILE_OFFSET_BITS=64 -D_LARGE_FILES -D__WXGTK__ -fno-rtti -fno-exceptions -fno-pcc-struct-return -fstrict-aliasing -Wall -fexceptions -s

all : $(OBJ)
	$(CC) $(OBJ) -o $(BIN) $(RES) $(LIBS) $(CFLAGS)

"app.o" :
	$(CC) -c "app.cpp" $(CFLAGS)

"api.o" :
	$(CC) -c "api.cpp" $(CFLAGS)
