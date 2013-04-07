sampleRate = 20000;
toneFrequency = 262;
tick_tone = sampleRate/toneFrequency;
degree_tick = 360/tick_tone;
MOD = 2*pi*toneFrequency/sampleRate;
amplitude = 2^12/2;
t = 1:sampleRate;
x = degree_tick .* t;
y = MOD .* t;
Bhaskara_degrees = (4.*x.*(180.-x))./(40500 .- x .* (180 .- x));
%
Bhaskara_degrees = amplitude + amplitude .*Bhaskara_degrees;
TrueSine = amplitude + amplitude .* sin(MOD.*t);
Error_Bhaskara_degrees = TrueSine .- Bhaskara_degrees;

n = tick_tone;
figure(1); plot(t(1:n),Bhaskara_degrees(1:n),";Bhaskara Approximation;",t(1:n),TrueSine(1:n),";Sine;",t(1:n),Error_Bhaskara_degrees(1:n),";Error;")
title(' Bhaskara Sine Approximation Compared to a True Sine Wave (Unadjusted)')
xlabel(' Number of Ticks (1 tick = .05ms)')
ylabel(' Digital value to be passed to DAC ')
print -deps comparison.eps

n = tick_tone;
m = tick_tone/2;
for t = 1:sampleRate,
	i = mod(t,n);
	h = mod(t,m);
	if (i == h)
		x = degree_tick * h;
		Bhaskara_degrees(t) = (4*x*(180-x))/(40500 - x * (180 - x));
		Bhaskara_degrees(t) = amplitude + amplitude *Bhaskara_degrees(t);
	else
		x = degree_tick * h;
		Bhaskara_degrees(t) = (4*x*(180-x))/(40500 - x * (180 - x));
		Bhaskara_degrees(t) = amplitude - amplitude *Bhaskara_degrees(t);
	endif
end


n = tick_tone;
t = 1:sampleRate;
Error_Bhaskara_degrees = TrueSine .- Bhaskara_degrees;
figure(2); plot(t(1:n),Bhaskara_degrees(1:n),";Bhaskara Approximation;",t(1:n),TrueSine(1:n),";Sine;",t(1:n),Error_Bhaskara_degrees(1:n),";Error;")
title(' Bhaskara Sine Approximation Compared to a True Sine Wave (Adjusted)')
xlabel(' Number of Ticks (1 tick = .05ms)')
ylabel(' Digital value to be passed to DAC ')
print -deps comparison2.eps




