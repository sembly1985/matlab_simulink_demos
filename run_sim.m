t=0:0.01:100;
u=rand(size(t));
sys=tf(1,[1 1]);
y=lsim(sys,u,t);
sim('m_run_sim');
plot(t,sim_y.Data,t,y);