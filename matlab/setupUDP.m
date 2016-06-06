function [s,ictb] = setupUDP(addr,port,timeout)

if nargin<3, timeout=1000; end
if nargin<2, port=[]; end

if license('test','matlab')
    s = java.net.DatagramSocket(port); % don't specify port if for sending
    s.setSoTimeout(timeout*1000)
    s.setReuseAddress(true)
    ictb=false;
elseif license('test','octave')
    error('contact author for updated script for UDP on Octave')
else
    error('unknown Matlab UDP setup')
end


end %function
