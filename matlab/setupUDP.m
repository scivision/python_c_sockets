function [s,ictb] = setupUDP(opt)
arguments
  opt.port = [] % don't specify port if for sending
  opt.timeout = 1000
end

s = java.net.DatagramSocket(opt.port);
s.setSoTimeout(opt.timeout*1000)
s.setReuseAddress(true)
ictb=false;

end %function
