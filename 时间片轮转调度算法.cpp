#include<iostream>
#include<string>
#include<vector>
#include<deque>
#include<queue>
#include<algorithm>

using namespace std;

class Time {
public:
	int hour;
	int minute;
	bool operator<(const Time& t) const {
		int t1 = this->hour * 60 + this->minute, t2 = t.hour * 60 + t.minute;
		if (t1 < t2)return true;
		return false;
	}
	bool operator>(const Time& t) const{
		int t1 = this->hour * 60 + this->minute, t2 = t.hour * 60 + t.minute;
		if (t1 > t2)return true;
		return false;
	}
	bool operator<=(const Time& t) const{
		if (*this > t)return false;
		return true;
	}
	void operator+=(const int dur) {
		this->minute += dur;
		if (this->minute >= 60) {
			this->hour += this->minute / 60;
			this->minute %= 60;
		}
		return;
	}
	void operator=(const Time& t) {
		this->hour = t.hour;
		this->minute = t.minute;
	}
	
	/*void func() const{ ; }*/
};
int operator-(const Time& t1, const Time& t2) {
	return (t1.hour - t2.hour) * 60 + t1.minute - t2.minute;
}
istream& operator>>(istream& in, Time& t) {
	char c;
	in >> t.hour >> c >> t.minute;
	return in;
}
class Process {
public:
	int id;//进程编号 
	string name;//进程名 
	Time arrive;//到达就绪队列的时间 
	int zx;//执行时间 

	Time start;//开始执行时间 
	Time finish;//执行完成时间 
	int zz;//周转时间=执行完成时间-到达就绪队列时间 
	double zzxs;//带权周转时间=周转时间/执行时间 
	Time current;//当前进程开始执行的时间 
	int ywcsj;//进程已经完成的时间 
	int sysj; //当前进程的剩余时间 
};
class Arrive_greater {
public:
	bool operator()(const Process* p1, const Process* p2) {
		if (p1->arrive > p2->arrive) {
			return true;
		}
		return false;
	}
};
class Op_system {
public:
	int pcs_num = 100;
	int tm_s = 8;
	Time now;
	vector<Process*> pcs_arr;
	deque<Process*> pcs_que;
	vector<Process*> done;
	Op_system() {
		
		cout << "请输入进程数量：";
		cin >> pcs_num;
		pcs_arr.reserve(pcs_num);
		done.reserve(pcs_num);
		cout << "请输入时间片时间：";
		cin >> tm_s;
		Process* tmp = NULL;
		cout << "请输入进程的参数：\n";
		cout << "id" << "    " << "名字" << "    " << "到达时间" << "    " << " 执行时间" << endl;
		for (int i = 0; i < pcs_num; i++) {
			tmp = new Process;
			cin >> tmp->id >> tmp->name >> tmp->arrive >> tmp->zx;
			tmp->current.hour = 0; tmp->current.minute = 0;
			tmp->ywcsj = 0;
			tmp->sysj = tmp->zx;
			pcs_arr.push_back(tmp);
		}
		sort(pcs_arr.begin(), pcs_arr.end(), Arrive_greater());//按到达时间从大到小排序
		now = pcs_arr.back()->arrive;
	}
	void show_que() {
		cout << "ID号" << "    " << "名字" << "    " << "到达时间" << "    " << "总执行时间" <<
			"    " << "当前开始时间" << "    " << "已完成时间" << "    " << "剩余完成时间" << endl;
		Process* tmp = pcs_que[0];
		cout << tmp->id << "     " << tmp->name << "\t" << flush;
		printf("%02d:%02d          %2d           %02d:%02d            %2d             %2d\n", tmp->arrive.hour,
			tmp->arrive.minute, tmp->zx, tmp->current.hour, tmp->current.minute, tmp->ywcsj, tmp->sysj);
		
		for (int i = 1; i < pcs_que.size(); i++) {
			tmp = pcs_que[i]; 
			cout << tmp->id << "     " << tmp->name << "\t" << flush;
			printf("%02d:%02d          %2d           00:00            %2d             %2d\n", tmp->arrive.hour,
				tmp->arrive.minute, tmp->zx, tmp->ywcsj, tmp->sysj);
		}
	}
	void show_done() {
		double sum_zzxs = 0;
		double sum_zz = 0;
		cout << "ID号" << "    " << "名字" << "    " << "到达时间" << "    " << "执行时间" <<
			"    " << "首次开始时间" << "    " << "完成时间" << "    " << "周转时间" << "   " << "带权周转系数" << endl;
		Process* tmp = done[0];
		for (int i = 0; i < done.size(); i++) {
			tmp = done[i];
			cout << tmp->id << "     " << tmp->name << "\t" << flush;
			printf("%02d:%02d          %2d         %02d:%02d         %02d:%02d         %2d          %2.2lf\n", tmp->arrive.hour,
				tmp->arrive.minute, tmp->zx, tmp->start.hour,tmp->start.minute, tmp->finish.hour,tmp->finish.minute,tmp->zz,tmp->zzxs);
			sum_zzxs += tmp->zzxs;
			sum_zz += tmp->zz;
		}
		printf("系统平均周转时间为：%.2lf\n", sum_zz / done.size());
		printf("系统平均周转系数为：%.2lf\n", sum_zzxs / done.size());
	}
	void run_pcs() {
		int cnt = 1;
		
		while (!pcs_arr.empty() || !pcs_que.empty()) {
			if (pcs_que.empty() && !pcs_arr.empty()) {
				now = pcs_arr.back()->arrive;
			}
			while (!pcs_arr.empty() && pcs_arr.back()->arrive <= now) {
				pcs_que.push_back(pcs_arr.back());
				pcs_arr.pop_back();
			}
			Process* que_front = pcs_que.front();
			int zx = 0;
			if ((que_front->sysj) < tm_s) {
				zx = que_front->sysj;
				que_front->finish = now;
				que_front->finish += zx;
				que_front->zz = que_front->finish - que_front->arrive;
				que_front->zzxs = 1.0 * que_front->zz / que_front->zx;
			}else {
				zx = tm_s;
			}
			if (que_front->ywcsj == 0) {
				que_front->start = now;
			}
			que_front->ywcsj += zx;
			que_front->sysj -= zx;
			que_front->current = now;
			cout << "第" << cnt++ << "轮执行和就绪队列结果：" << endl;
			
			now += zx;
			if (pcs_que.empty() && !pcs_arr.empty()) {
				now = pcs_arr.back()->arrive;
			}
			while (!pcs_arr.empty() && pcs_arr.back()->arrive <= now) {
				pcs_que.push_back(pcs_arr.back());
				pcs_arr.pop_back();
			}
			show_que();
			if (que_front->sysj == 0)
				done.push_back(que_front);
			else 
				pcs_que.push_back(que_front);
			pcs_que.pop_front();
		}
		cout << "模拟进程时间片轮转进程调度过程输出结果：" << endl;
		show_done();
		del_done();
	}
	void del_done() {
		for (int i = done.size() - 1; i >= 0; i--) {
			delete done[i];
			done.pop_back();
		}
	}
};

int main() {
	int flag = 1;
	while (true) {
		cout << "请输入操作：(1：开始进程；0：结束进程)：";
		cin >> flag;
		if (flag == 0)break;
		Op_system sys;
		sys.run_pcs();
	}
	
	system("pause");
	return 0;
}