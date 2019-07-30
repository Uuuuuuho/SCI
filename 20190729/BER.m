Files = dir('*-BER.txt'); 
numfiles = length(Files);
mydata = cell(1, numfiles);

for k = 1:numfiles 
  mydata{k} = load(Files(k).name); 
end

figure

for k = 1:numfiles 
  switch(k)
      case 1
        semilogy(mydata{1}(:,1), mydata{1}(:,2),'m+-')
        hold on
      case 2
        semilogy(mydata{2}(:,1), mydata{2}(:,2),'m>-')
      case 3
        semilogy(mydata{3}(:,1), mydata{3}(:,2),'r+-')
      case 4
        semilogy(mydata{4}(:,1), mydata{4}(:,2),'r>-')
  end
end

for k = 1:numfiles 
  switch(k)
      case 1
        legend(Files(1).name)
        hold on
      case 2
        legend(Files(1).name, Files(2).name)
      case 3
        legend(Files(1).name, Files(2).name, Files(3).name)
      case 4
        legend('Conventional \alpha 0.7, \gamma_R_D 0dB',...
            'Conventional \alpha 0.7, \gamma_R_D 5dB',...
            'Proposed \alpha 0.3, \gamma_R_D 0dB',...
            'Proposed \alpha 0.3, \gamma_R_D 5dB')
  end
end


%title('RD Gain 0dB, alpha 0.7');
xlabel('Eb/No, dB');
ylabel('Bit Error Rate');

grid on;