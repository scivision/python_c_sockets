
#---- What the EXE filename should be ---------------------------------
TARGET_uni = unicast_tx.out
TARGET_multitx = multicast_tx.out
TARGET_multirx = multicast_rx.out 
TARGET_h5 = unicast_rx_h5.out
#----Sources (in order of dependency) ---------------------------------
SRC_uni = unicast_tx.c
SRC_h5 = unicast_rx_h5.c
SRC_multitx = multicast_tx.c 
SRC_multirx = multicast_rx.c
#----- libs you need --------------------------------------------------
CLIBS = 
#------ C ----------------- -------------------------------------------
H5CC = h5cc  #just a bash script loading libraries and paths from libhdf5-dev
CFLAGS = -std=c11 -Wall -Wpedantic -Wextra -mtune=native -O3
#------ targets --------------------------------
all: $(TARGET_uni) $(TARGET_multitx) $(TARGET_multirx) $(TARGET_h5)

unicast: $(TARGET_uni)

multicast: $(TARGET_multitx) $(TARGET_multirx)

h5: $(TARGET_h5)

$(TARGET_uni): $(SRC_uni)
	$(CC) -o $@ $(CFLAGS) $(SRC_uni) $(CLIBS) $(LDFLAGS) 

$(TARGET_h5): $(SRC_h5)
	$(H5CC) -o $@ $(CFLAGS) $(SRC_h5)

$(TARGET_multitx): $(SRC_multitx) 
	$(CC) -o $@ $(CFLAGS) $(SRC_multitx) $(CLIBS) $(LDFLAGS) 

$(TARGET_multirx): $(SRC_multirx) 
	$(CC) -o $@ $(CFLAGS) $(SRC_multirx) $(CLIBS) $(LDFLAGS) 
   
#----------- CLEAN ----------------------------
clean:
	$(RM) $(TARGET_multitx) $(TARGET_multirx) $(TARGET_uni)

