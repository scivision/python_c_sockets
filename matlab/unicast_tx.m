% does not require Instrument Control Toolbox
% for now, uses Matlab (not Octave)

addr = 'localhost';
port = 2000;

msg = int8('hello');

addr = java.net.InetAddress.getByName(addr);

s = setupUDP(addr);

pkt = java.net.DatagramPacket(msg,length(msg),addr,port);
s.send(pkt)

s.close
