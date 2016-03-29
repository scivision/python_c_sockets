
#---- What the EXE filename should be ---------------------------------
unitx = unicast_tx.out
unirx = unicast_rx.out
multitx = multicast_tx.out
multirx = multicast_rx.out 
h5 = unicast_rx_h5.out
#----Sources (in order of dependency) ---------------------------------
s_unitx = unicast_tx.c
s_unirx = unicast_rx.c
s_h5 = unicast_rx_h5.c
s_multitx = multicast_tx.c 
s_multirx = multicast_rx.c
#----- libs you need --------------------------------------------------
CLIBS = 
#------ C ----------------- -------------------------------------------
H5CC = h5cc  #just a bash script loading libraries and paths from libhdf5-dev
CFLAGS = -std=c11 -Wall -Wpedantic -Wextra -mtune=native -O3
#------ targets --------------------------------
all: $(unitx) $(unirx) $(multitx) $(multirx) $(h5)

unicast: $(unitx) $(unirx)

multicast: $(multitx) $(multirx)

h5: $(h5)

$(unitx): $(s_unitx)
	$(CC) -o $@ $(CFLAGS) $(s_unitx) $(CLIBS) $(LDFLAGS) 

$(unirx): $(s_unirx)
	$(CC) -o $@ $(CFLAGS) $(s_unirx) $(CLIBS) $(LDFLAGS) 

$(h5): $(s_h5)
	$(H5CC) -o $@ $(CFLAGS) $(s_h5) $(CLIBS) $(LDFLAGS) 

$(multitx): $(s_multitx) 
	$(CC) -o $@ $(CFLAGS) $(s_multitx) $(CLIBS) $(LDFLAGS) 

$(multirx): $(s_multirx) 
	$(CC) -o $@ $(CFLAGS) $(s_multirx) $(CLIBS) $(LDFLAGS) 
   
#----------- CLEAN ----------------------------
clean:
	$(RM) $(multitx) $(multirx) $(unitx) $(unirx) $(h5)

