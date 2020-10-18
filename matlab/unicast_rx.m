% Simple send/receive to companion UDP C-code server on localhost or other device
% prints rolling mean of cycle time (typical <80 microseconds on localhost)
%
% It does 2 UDP reads per cycle:
% 1) get number of float32 via int
% 2) get float32 array

function unicast_rx(N, opt)
arguments
  N (1,1) {mustBeInteger,mustBePositive} % number of total packets to read
  opt.host (1,1) string = '::1'  % '::1' is to ipv6 what 'localhost' is to ipv4
  opt.port (1,1) {mustBeInteger,mustBePositive} = 2000
  opt.buffer (1,1) {mustBeInteger,mustBePositive} = 8192
  opt.Nel (1,1) {mustBeInteger,mustBePositive} = 2048 % number of elements per packet
end

S = udp(opt.host, opt.port, 'InputBufferSize', opt.buffer, ...
    'DatagramTerminateMode','on'); %8192 byte max read at one time
S.timeout=0.2;
fopen(S);
assert(strcmp(S.Status,'open'),'no connection on socket')
% Do NOT connect or bind
first=true;

for i = 1:N-1
  tic
  %% host -> device
  fwrite(S,'\n')
  %% device -> host

  fread(S,1,'uint32');
  dat = fread(S, opt.Nel,'float32');

  if length(dat) ~= opt.Nel
    disp(length(dat))
    continue
  end
%% parse result
  if first %to avoid having to restart C code each time
    first = false;
    % last = dat(1)-1;
    rtoc = toc;
  else
    rtoc = mean([toc,rtoc]);
  end

  %if ~mod(i, 1000), disp(rtoc), end

  %(last==dat(1)-1,num2str(last))
  % last = dat(end);
end

fclose(S);

end % function
