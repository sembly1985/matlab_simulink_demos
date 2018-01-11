syms s R L C; % 定义运算符号
% 计算阻抗
Z_R=R; 
Z_L=s*L; 
Z_C = 1/(s*C);
% 计算 uo 与 ui见的传递函数
ui2uo = Z_C/(Z_R+Z_L+Z_C);
% 使用 collect指令化简
ui2uo_s=collect(ui2uo,'s')
% subs 指令 替换 RLC为实际值
sys = subs(ui2uo_s,{'R','L','C'},[1e3,1e-3,1e-6])