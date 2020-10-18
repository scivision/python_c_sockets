% does not require Instrument Control Toolbox

addr = '::1';
port = 2000;

msg = int8('hello');

addr = java.net.InetAddress.getByName(addr);

s = setupUDP("port", port);

pkt = java.net.DatagramPacket(msg,length(msg), addr, port);
s.send(pkt)

s.close
