% Simple send/receive to companion UDP C-code server on localhost or other device
% median cycle time <80 microseconds on localhost
%
% It does 2 UDP reads per cycle:
% 1) get number of float32 via int
% 2) get float32 array

function unicast_rx(N, opt)
arguments
  N (1,1) {mustBeInteger,mustBePositive} % number of total packets to read
  opt.host {mustBeTextScalar} = '::1'  % '::1' is to ipv6 what 'localhost' is to ipv4
  opt.port (1,1) {mustBeInteger,mustBePositive} = 2001
  opt.Nel (1,1) {mustBeInteger,mustBePositive} = 2048 % number of elements per packet, 8192 byte max read at one time
end

S = udpport("datagram", "IPv6", LocalHost=opt.host, LocalPort=opt.port, Timeout=0.2); 

assert(strcmp(S.Status,'open'), 'no connection on socket')
% Do NOT connect or bind

rtoc = zeros(N-1, 1);

for i = 1:N-1
  tic
  %% host -> device
  write(S, '\n', 'string', opt.host, opt.port)
  %% device -> host

  read(S, 1, 'uint32');
  dat = read(S, opt.Nel, 'float32');

  if length(dat) ~= opt.Nel
    fprintf('unexpected length %d != %d\n', length(dat), opt.Nel)
    continue
  end

  rtoc(i) = toc;
end

flush(S, "input")
delete(S)

fprintf('Median packet time %f\n', median(rtoc))

end
