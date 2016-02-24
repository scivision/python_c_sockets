% Simple send/receive to companion UDP C-code server on localhost or other device
% prints rolling mean of cycle time (typical <80 microseconds on localhost)
% 
% It does 2 UDP reads per cycle:
% 1) get number of float32 via int
% 2) get float32 array

function socksink()
HOST='::1'; % '::1' is to ipv6 what 'localhost' is to ipv4
PORT=2000;
BUFSIZE=8192;
Nelbyte = 4; %4 bytes for float32


    S = udp(HOST,PORT,'InputBufferSize',BUFSIZE,'DatagramTerminateMode','on'); %8192 byte max read at one time
    S.timeout=0.1;
    fopen(S);
    % Do NOT connect or bind
      first=true;

      for i =1:100000000
        tic
%% host-> device
        fwrite(S,'\n')
%% device -> host
        Nel = fread(S,4,'int32') %int len
        dat = fread(S,Nel*Nelbyte,'float32');

        if isempty(dat)
            continue
        end
%% parse result
        if first %to avoid having to restart C code each time
            first = false;
            last = dat(1)-1;
            rtoc = toc;
        else
            rtoc = mean([toc,rtoc]);
        end
        
        if ~mod(i, 1000), disp(rtoc), end

        assert(last==dat(1)-1,num2str(last))
        last = dat(end);
      end

    fclose(S);

end
