#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <fstream>
#include <ctime>
using namespace std;
const int SO_CAU=15;
const long THUONG[SO_CAU]={200, 400, 600, 1000, 2000, 3000, 6000, 10000,14000, 22000, 30000, 40000, 60000, 85000, 150000};
template <class T>
class DanhSach {
    vector<T> ds;
    public:
    void them(T x) {ds.push_back(x);}
    int SoLuong() {return ds.size();}
    T& operator[](int i) {return ds[i];}
};
class CauHoi {
    protected:
    string noiDung;
    int dapAn;
    bool an[4];
    public:
    CauHoi(string nd, int da) {noiDung=nd;dapAn=da;datLai();}
    virtual ~CauHoi() {}
    virtual void hienThi()=0;
    virtual int soLuaChon()=0;
    virtual bool boBot() {return false;}
    bool kiemTrA(int chon) {return chon==dapAn;}
    int layDapAn() {return dapAn;}
    bool daBiAn(int c) {return c>=1 && c<=4 && an[c-1];}
    void datLai() {for(int i=0;i<4;i++) an[i]=false;}
};
class CauHoiABCD:public CauHoi {
    string pa[4];
    public:
    CauHoiABCD(string nd, string a, string b, string c, string d, int da):CauHoi(nd,da) {
        pa[0]=a;pa[1]=b;pa[2]=c;pa[3]=d;
    }
    void hienThi() {
        cout<<noiDung<<endl;
        for(int i=0;i<4;i++) {
            if(!an[i]) cout<<" "<<i+1<<". "<<pa[i]<<endl;
        }
    }
    int soLuaChon(){return 4;}
    bool boBot() {
        int giu=rand()%3;
        int dem=0;
        for(int i=0;i<4;i++) {
            if(i+1==dapAn) continue;
            if(dem!=giu) an[i]=true;
            dem++;
        }
        return true;
    }
};
class CauHoiDungSai:public CauHoi {
    public:
    CauHoiDungSai(string nd, int da):CauHoi(nd,da) {}
    void hienThi() {
        cout<<noiDung<<endl;
        cout<<"  1.Đúng\n  2.Sai\n";
    }
    int soLuaChon() {return 2;}
};
int doanDapAn(CauHoi *ch,int tl) {
    if(rand()%100<tl) return ch->layDapAn();
    int g;
    do{g=1+rand()%ch->soLuaChon();} while(ch->daBiAn(g));
    return g;
}
class TroGiup {
    protected:
    string ten;
    bool daDung;
    public:
    TroGiup(string t) {ten=t;daDung=false;}
    virtual ~TroGiup() {}
    virtual bool thucHien(CauHoi *ch)=0;
    string layTen() {return ten;}
    bool conDung() {return !daDung;}
    void dung(CauHoi *ch) {if(thucHien(ch)) daDung=true;}
};
class TroGiup5050:public TroGiup {
    public:
    TroGiup5050():TroGiup("50:50") {}
    bool thucHien(CauHoi *ch) {
        if(!ch->boBot()) {
            cout<<"50:50 không được áp dụng cho câu Đúng/Sai"<<endl;
            return false;
        }
        cout<<"Đã loại bỏ 2 đáp án sai!"<<endl;
        return true;
    }
};
class GoiNguoiThan:public TroGiup {
    public:
    GoiNguoiThan():TroGiup("Gọi người thân") {}
    bool thucHien(CauHoi *ch) {
        cout<<"Người thân: Có thể đáp án là "<<doanDapAn(ch,80)<<endl;
        return true;
    }
};
class HoiKhanGia:public TroGiup {
    public:
    HoiKhanGia():TroGiup("Hỏi khán giả") {}
    bool thucHien(CauHoi *ch) {
        int dung=ch->layDapAn();
        int tl[4]={0,0,0,0};
        int con=100-(40+rand()%31);
        tl[dung-1]=100-con;
        int soSai=0;
        for(int i=0;i<ch->soLuaChon();i++) {
            if(i+1!=dung && !ch->daBiAn(i+1)) soSai++;
        }
        int dem=0;
        for(int i=0;i<ch->soLuaChon(); i++) {
            if(i+1==dung || ch->daBiAn(i+1)) continue;
            dem++;
            if(dem==soSai) tl[i]=con;
            else {tl[i]=rand()%(con+1);con-=tl[i];}
        }
        cout<<"Khán giả:\n";
        for(int i=0;i<ch->soLuaChon();i++) {
            if(!ch->daBiAn(i+1)) cout<<"Đáp án "<<i+1<<": "<<tl[i]<<"%\n";
        }
        return true;
    }
};
class ChuyenGia:public TroGiup {
    public:
    ChuyenGia():TroGiup("Hỏi chuyên gia") {}
    bool thucHien(CauHoi *ch) {
        for(int i=1;i<=3;i++) cout<<"Chuyên gia "<<i<<" chọn đáp án "<<doanDapAn(ch,70)<<endl;
        return true;
    }
};
class NguoiChoi {
    string ten;
    int soCauDung;
    long tien;
    public:
    NguoiChoi(string t="") {ten=t;soCauDung=0;tien=0;}
    void ketThuc(int dung, long t) {soCauDung=dung;tien=t;}
    long layTien() {return tien;}
    void xuat() {
        cout<<ten<<" | Đúng "<<soCauDung<<" Câu | Thưởng "<<tien<<" nghìn đồng "<<endl;
    }
};
class TroChoi {
    DanhSach<CauHoi*> dsCau;
    DanhSach<NguoiChoi> lichSu;
    string tenFile;
    public:
    long mocAnToan(int i) {
        if(i>=10) return THUONG[9];
        if(i>=5) return THUONG[4];
        return 0;
    }
    public:
    TroChoi(string file) {
        tenFile=file;
        docFile();
    }
    ~TroChoi() {
        for(int i=0;i<dsCau.SoLuong();i++) delete dsCau[i];
    }
    void docFile() {
        ifstream f(tenFile.c_str());
        if(!f) {
            cout<<"Không thể mở file"<<endl;
            return;
        }
        string dong,nd,a,b,c,d,s;
        while(getline(f,dong)) {
            if(dong=="") continue;
            int loai=atoi(dong.c_str());
            getline(f,nd);
            if(loai==1) {
                getline(f,a);getline(f,b);getline(f,c);getline(f,d);getline(f,s);
                dsCau.them(new CauHoiABCD(nd,a,b,c,d,atoi(s.c_str())));
            } else {
                getline(f,s);
                dsCau.them(new CauHoiDungSai(nd,atoi(s.c_str())));
            }
        }
    }
    void choi() {
        int n=dsCau.SoLuong();
        if(n==0) {
            cout<<"Chưa có câu hỏi"<<endl;
            return;
        }
        int soCau=(n<SO_CAU)?n:SO_CAU;
        //random câu hỏi
        DanhSach<int> idx;
        for(int i=0;i<n;i++) idx.them(i);
        for(int i=0;i<soCau;i++) {
            int j=i+rand()%(n-i);
            int temp=idx[i];idx[i]=idx[j];idx[j]=temp;
        }
        string ten;
        cin.ignore();
        cout<<"Nhập tên người chơi: ";
        getline(cin,ten);
        TroGiup *tg[4]={new TroGiup5050(), new GoiNguoiThan(), new HoiKhanGia(), new ChuyenGia()};
        int dung=0;
        long tien=0;
        for(int i=0;i<soCau;i++) {
            CauHoi *ch=dsCau[idx[i]];
            ch->datLai();
            cout<<"\n--- Câu "<<i+1<<" ("<<THUONG[i]<< " nghìn đồng) ---\n"; 
            int chon;
            while(true) {
                ch->hienThi();
                cout<<"Trợ giúp còn lại: ";
                for(int k=0;k<4;k++)
                    if(tg[k]->conDung()) cout<<k+5<<"."<<tg[k]->layTen()<<"  ";
                cout<<"\nChọn đáp án (0 = Dừng chơi): ";
                if(!(cin>>chon)) {
                    cin.clear();
                    cin.ignore(1000,'\n');
                    chon=-1;
                }
                if(chon>=5 && chon<=8) {
                    if(tg[chon-5]->conDung()) tg[chon-5]->dung(ch);
                    else cout<<"Đã sử dụng quyền trợ giúp này!"<<endl;
                }
                else if(chon==0 || (chon>=1 && chon<=ch->soLuaChon() && !ch->daBiAn(chon)))
                    break;
            }
            if(chon==0) break;
            if(ch->kiemTrA(chon)) {
                cout<<"Chính xác!"<<endl;
                dung++;
                tien=THUONG[i];
            } else {
                cout<<"Sai!"<<endl;
                tien=mocAnToan(i);
                break;
            }
        }
        for(int i=0;i<4;i++) delete tg[i];
        if(dung==soCau) cout<<"\nChúc mừng bạn đã chiến thắng!\n";
        NguoiChoi nc(ten);
        nc.ketThuc(dung,tien);
        cout<<"\nKết thúc\n";
        nc.xuat();
        lichSu.them(nc);
    }
    void themCauHoi() {
        int loai,da;
        string nd,a,b,c,d;
        cout<<"Loại câu hỏi(1. 4 đáp án,2. Đúng sai): ";
        cin>>loai;
        cin.ignore();
        cout<<"Nội dung câu hỏi: ";
        getline(cin,nd);
        ofstream f(tenFile.c_str(),ios::app);
        f<<endl;
        if(loai==1) {
            cout<<"Đáp án 1: ";getline(cin, a);
            cout<< "Đáp án 2: ";getline(cin, b);
            cout<< "Đáp án 3: ";getline(cin, c);
            cout<< "Đáp án 4: ";getline(cin, d);
            cout<< "Đáp án đúng (1-4): ";cin>>da;
            dsCau.them(new CauHoiABCD(nd,a,b,c,d,da));
            f<<1<<endl<<nd<<endl<<a<<endl<<b<<endl<<c<<endl<<d<<endl<<da<<endl;
        } else {
            cout<<"Đáp án đúng(1.Đúng, 2.Sai): ";cin>>da;
            dsCau.them(new CauHoiDungSai(nd,da));
            f<<2<<endl<<nd<<endl<<da<<endl;
        }
        cout<<"Đã thêm câu hỏi"<<endl;
    }
    void thongKe() {
        int n=lichSu.SoLuong();
        if(n==0) {
            cout<<"Chưa có lượt chơi"<<endl;
            return;
        }
        long tong=0,cao=0;
        for(int i=0;i<n;i++) {
            lichSu[i].xuat();
            tong+=lichSu[i].layTien();
            if(lichSu[i].layTien()>cao) cao=lichSu[i].layTien();
        }
        cout<<"Tổng số lượt chơi:"<<n<< endl;
        cout<<"Tiền thưởng cao nhất: "<<cao<<" nghìn đồng"<<endl;
        cout<<"Tiền thưởng trung bình: "<<tong/n<<" nghìn đồng"<<endl;
    }
};
int main() {
    system("chcp 65001 > nul");
    srand(time(0));
    TroChoi game("cauhoi.txt");
    int chon;
    do {
        cout<<"\n===== AI LÀ TRIỆU PHÚ =====\n";
        cout<<"1. Chơi game\n2. Thêm câu hỏi\n3. Thống kê\n0. Thoát\nChọn: ";
        if(!(cin>>chon)) {
            cin.clear();
            cin.ignore(1000,'\n');
            chon=-1;  
        }
        if(chon==1) game.choi();
        else if(chon==2) game.themCauHoi();
        else if(chon==3) game.thongKe();
    } while(chon!=0);
    return 0;
}
