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
        semilogy(mydata{1}(:,1), mydata{1}(:,2),'m-*')
        hold on
      case 2
        semilogy(mydata{2}(:,1), mydata{2}(:,2),'r-o')
      case 3
        semilogy(mydata{3}(:,1), mydata{3}(:,2),'g+')
      case 4
        semilogy(mydata{4}(:,1), mydata{4}(:,2),'k>')
      case 5
        semilogy(mydata{4}(:,1), mydata{4}(:,2),'b-*')
      case 6
        semilogy(mydata{4}(:,1), mydata{4}(:,2),'b>')
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
        legend('\alpha_1 0.6', '\alpha_1 0.7', '\alpha_1 0.8', '\alpha_1 0.9')
      case 5
        legend(Files(1).name, Files(2).name, Files(3).name, Files(4).name, Files(5).name)  
        %'Iter New Switch \alpha_1 0.9',...
         %   'Iter Conv \alpha_1 0.6', 'Iter Switch \alpha_1 0.9',...
          %  'LCSIC Conv \alpha_1 0.6', 'LCSIC New Switch \alpha_1 0.6','LCSIC Switch \alpha_1 0.6'
      case 6
        legend('Iter Conv \alpha_1 0.6','Iter New Switch \alpha_1 0.9',...
            'Iter Switch \alpha_1 0.9', 'LCSIC Conv \alpha_1 0.6',...
            'LCSIC New Switch \alpha_1 0.6','LCSIC Switch \alpha_1 0.6') 
        %legend(Files(1).name, Files(2).name, Files(3).name, Files(4).name, Files(5).name, Files(6).name)
  end
end


title({'BER Comparison over Quasi-static Rayleigh fading channel'});
xlabel('Eb/No, dB');
ylabel('Bit Error Rate');

grid on;