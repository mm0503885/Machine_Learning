Input_Matrix = csvread('data.txt');
x=Input_Matrix(:,1);
y=Input_Matrix(:,2);
Input_Size = size(Input_Matrix);
Input_num = Input_Size(1);

%case1
%計算Design_Matrix
Design_Matrix1=zeros(Input_num,2);
for i=1:Input_num
Design_Matrix1(i,1)=Input_Matrix(i,1);
Design_Matrix1(i,2)=1;
end
Design_Matrix1_T=Design_Matrix1.';
%計算ATA
ATA=Design_Matrix1_T*Design_Matrix1;
%計算L,U矩陣
L=zeros(2,2);
L(1,1)=1;
L(2,2)=1;
U=ATA;
s=ATA(2,1)/ATA(1,1);
U(2,1)=ATA(2,1)-ATA(1,1)*s;
U(2,2)=ATA(2,2)-ATA(1,2)*s;
L(2,1)=s;
%計算L,U矩陣的inverse
LI=L;
LI(2,1)=-L(2,1);
UI=U;
UI(1,1)=1/U(1,1);
UI(2,2)=1/U(2,2);
det=U(1,1)*U(2,2);
UI(1,2)=(-U(1,2))/det;
ATAI=UI*LI;
final=ATAI*Design_Matrix1_T*y;

%case2
%計算Design_Matrix
Design_Matrix2=zeros(Input_num,3);
for i=1:Input_num
Design_Matrix2(i,1)=Input_Matrix(i,1)^2;
Design_Matrix2(i,2)=Input_Matrix(i,1);
Design_Matrix2(i,3)=1;
end

Design_Matrix2_T=Design_Matrix2.';
%計算ATA
ATA2=Design_Matrix2_T*Design_Matrix2;
%計算L,U矩陣
L2=zeros(3,3);
L2(1,1)=1;
L2(2,2)=1;
L2(3,3)=1;
U2=ATA2;
s2_1=ATA2(2,1)/ATA2(1,1);
s2_2=ATA2(3,1)/ATA2(1,1);
U2(2,1)=ATA2(2,1)-ATA2(1,1)*s2_1;
U2(2,2)=ATA2(2,2)-ATA2(1,2)*s2_1;
U2(2,3)=ATA2(2,3)-ATA2(1,3)*s2_1;
U2(3,1)=ATA2(3,1)-ATA2(1,1)*s2_2;
U2(3,2)=ATA2(3,2)-ATA2(1,2)*s2_2;
U2(3,3)=ATA2(3,3)-ATA2(1,3)*s2_2;
s2_3=U2(3,2)/U2(2,2);
U2(3,2)=U2(3,2)-U2(2,2)*s2_3;
U2(3,3)=U2(3,3)-U2(2,3)*s2_3;
L2(2,1)=s2_1;
L2(3,1)=s2_2;
L2(3,2)=s2_3;

%計算L,U矩陣的inverse
L2I=L2;
t2_1=-s2_1;
t2_2=-s2_2;
t2_3=-s2_3;
L2I(2,1)=t2_1;
L2I(3,1)=t2_2+L2I(2,1)*t2_3;
L2I(3,2)=t2_3;



U2I=U2;
U2I(1,1)=1/U2(1,1);
U2I(2,2)=1/U2(2,2);
U2I(3,3)=1/U2(3,3);
det2=U2(1,1)*U2(2,2)*U2(3,3);
U2I(1,2)=U2(1,2)*U2(3,3)/det2;
U2I(1,2)=-U2I(1,2);
U2I(2,3)=U2(2,3)*U2(1,1)/det2;
U2I(2,3)=-U2I(2,3);
U2I(1,3)=U2(1,2)*U2(2,3)-U2(2,2)*U2(1,3);
U2I(1,3)=U2I(1,3)/det2;
ATA2I=U2I*L2I;
final2=ATA2I*Design_Matrix2_T*y;

m = linspace(-5, 5);
n1 = final(1)*m+final(2);
n2 = final2(1)*m.^2+final2(2)*m+final2(3);

%case3
regulation=zeros(3,3);
regulation(1,1)=10000;
regulation(2,2)=10000;
regulation(3,3)=10000;
ATA3=ATA2+regulation;
%計算L,U矩陣
L3=zeros(3,3);
L3(1,1)=1;
L3(2,2)=1;
L3(3,3)=1;
U3=ATA3;
s3_1=ATA3(2,1)/ATA3(1,1);
s3_2=ATA3(3,1)/ATA3(1,1);
U3(2,1)=ATA3(2,1)-ATA3(1,1)*s3_1;
U3(2,2)=ATA3(2,2)-ATA3(1,2)*s3_1;
U3(2,3)=ATA3(2,3)-ATA3(1,3)*s3_1;
U3(3,1)=ATA3(3,1)-ATA3(1,1)*s3_2;
U3(3,2)=ATA3(3,2)-ATA3(1,2)*s3_2;
U3(3,3)=ATA3(3,3)-ATA3(1,3)*s3_2;
s3_3=U3(3,2)/U3(2,2);
U3(3,2)=U3(3,2)-U3(2,2)*s3_3;
U3(3,3)=U3(3,3)-U3(2,3)*s3_3;
L3(2,1)=s3_1;
L3(3,1)=s3_2;
L3(3,2)=s3_3;

%計算L,U矩陣的inverse
L3I=L3;
t3_1=-s3_1;
t3_2=-s3_2;
t3_3=-s3_3;
L3I(2,1)=t3_1;
L3I(3,1)=t3_2+L3I(2,1)*t3_3;
L3I(3,2)=t3_3;



U3I=U3;
U3I(1,1)=1/U3(1,1);
U3I(2,2)=1/U3(2,2);
U3I(3,3)=1/U3(3,3);
det3=U3(1,1)*U3(2,2)*U3(3,3);
U3I(1,2)=U3(1,2)*U3(3,3)/det3;
U3I(1,2)=-U3I(1,2);
U3I(2,3)=U3(2,3)*U3(1,1)/det3;
U3I(2,3)=-U3I(2,3);
U3I(1,3)=U3(1,2)*U3(2,3)-U3(2,2)*U3(1,3);
U3I(1,3)=U3I(1,3)/det3;
ATA3I=U3I*L3I;
final3=ATA3I*Design_Matrix2_T*y;

m = linspace(-6, 6);
n1 = final(1)*m+final(2);
n2 = final2(1)*m.^2+final2(2)*m+final2(3);
n3 = final3(1)*m.^2+final3(2)*m+final3(3);

total_error1=0;
total_error2=0;
total_error3=0;
for i=1:Input_num
  total_error1=total_error1+(final(1)*x(i)+final(2)-y(i))^2;
  total_error2=total_error2+(final2(1)*x(i)^2+final2(2)*x(i)+final2(3)-y(i))^2;
  total_error3=total_error3+(final3(1)*x(i)^2+final3(2)*x(i)+final3(3)-y(i))^2;
end

subplot(2, 3, 1);
plot(x,y,'o',m,n1);
title('Case 1,LSE');
subplot(2, 3, 4);
plot(x,y,'o',m,n1);
title('Case 1,Newton''s Method');
fprintf('Case 1: n = 2, λ= 0\n');
fprintf('LSE:\n');
fprintf('Fitting line: %.14fX^1 + %.14f\n',final(1),final(2));
fprintf('Total error: %.14f\n\n',total_error1);
fprintf('Newton''s Method:\n'); 
fprintf('Fitting line: %.14fX^1 + %.14f\n',final(1),final(2));
fprintf('Total error: %.14f\n\n',total_error1);

subplot(2, 3, 2);
plot(x,y,'o',m,n2);
title('Case 2,LSE');
subplot(2, 3, 5);
plot(x,y,'o',m,n2);
title('Case 2,Newton''s Method');
fprintf('Case 2: n = 3, λ= 0\n');
fprintf('LSE:\n');
fprintf('Fitting line: %.14fX^2 +  %.14fX^1 - %.14f\n',final2(1),final2(2),-final2(3));
fprintf('Total error: %.14f\n\n',total_error2);
fprintf('Newton''s Method:\n');
fprintf('Fitting line: %.14fX^2 +  %.14fX^1 - %.14f\n',final2(1),final2(2),-final2(3));
fprintf('Total error: %.14f\n\n',total_error2);

subplot(2, 3, 3);
plot(x,y,'o',m,n3);
title('Case 3,LSE');
subplot(2, 3, 6);
plot(x,y,'o',m,n2);
title('Case 3,Newton''s Method');
fprintf('Case 3: n = 3, λ= 10000\n');
fprintf('LSE:\n');
fprintf('Fitting line: %.14fX^2 +  %.14fX^1 + %.14f\n',final3(1),final3(2),final3(3));
fprintf('Total error: %.14f\n\n',total_error3);
fprintf('Newton''s Method:\n');
fprintf('Fitting line: %.14fX^2 +  %.14fX^1 - %.14f\n',final2(1),final2(2),-final2(3));
fprintf('Total error: %.14f\n\n',total_error2);







