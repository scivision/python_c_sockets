function [s,ictb] = setupUDP(port,timeout)
narginchk(0,2)
if nargin<2, timeout=1000; end
if nargin<1, port=[]; end

validateattributes(timeout, {'numeric'}, {'positive'}, 2)

s = java.net.DatagramSocket(port); % don't specify port if for sending
s.setSoTimeout(timeout*1000)
s.setReuseAddress(true)
ictb=false;

end %function
