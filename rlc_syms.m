syms s R L C; % �����������
% �����迹
Z_R=R; 
Z_L=s*L; 
Z_C = 1/(s*C);
% ���� uo �� ui���Ĵ��ݺ���
ui2uo = Z_C/(Z_R+Z_L+Z_C);
% ʹ�� collectָ���
ui2uo_s=collect(ui2uo,'s')
% subs ָ�� �滻 RLCΪʵ��ֵ
sys = subs(ui2uo_s,{'R','L','C'},[1e3,1e-3,1e-6])