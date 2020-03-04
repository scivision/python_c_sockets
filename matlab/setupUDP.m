function [s,ictb] = setupUDP(port,timeout)

if nargin<2, timeout=1000; end
if nargin<1, port=[]; end

if isoctave
  error('contact author for updated script for UDP on Octave')
else
  s = java.net.DatagramSocket(port); % don't specify port if for sending
  s.setSoTimeout(timeout*1000)
  s.setReuseAddress(true)
  ictb=false;
end

end %function
