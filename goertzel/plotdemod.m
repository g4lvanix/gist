f = fopen("demod.raw");
data = fread(f,Inf,"uint16");
plot(data);
