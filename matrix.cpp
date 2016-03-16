#include <iostream>
using namespace std;

class AbstractMatrix
{
protected:
    int n;
    int m;
    float* data;
public:
//	AbstractMatrix();
//	AbstractMatrix(int m, int n);
    virtual AbstractMatrix& operator+(AbstractMatrix&)=0;
    virtual AbstractMatrix& operator*(AbstractMatrix&)=0;
    virtual AbstractMatrix& operator*(float&)=0;
    virtual AbstractMatrix& operator-(AbstractMatrix&)=0;
    virtual AbstractMatrix& reverse()=0;
    virtual AbstractMatrix& transpose()=0;
    virtual float determinant()=0;
    virtual ostream& print(ostream& o)=0;
    virtual istream& read(istream& o)=0;
    virtual void set(int i, int j, float data)=0;
    virtual float get(int i, int j)=0;
    virtual int getN()=0;
    virtual int getM()=0;
    virtual bool failed()=0;
};

class MyMatrix : public AbstractMatrix
{
    int getRightPos(int i, int j, int n)
    {
        return (n*i+j);
    }
    int getVertical(int pos, int n)
    {
        return (pos%n);
    }
    int getHorizontal(int pos, int n)
    {
        return (pos/n);
    }
    float* createNumbers(int m, int n)
    {
        if(m<=0 || n<=0)
            return NULL;
        float* _data = new float [m*n];
        for(int i=0; i<m*n; i++)
            _data[i] = 0;
        return _data;
    }
    void determinant(float det, int n, float data[], float* deter)
    {
        if (n == 0)
        {
            *deter += det;
            return;
        }
        for (int k = 0; k < n; k++)
        {
            if ((n + k + 1) % 2 == 1)
                det = -(det);
            float* dataNew = new float[(n - 1)*(n - 1) + 1];
            int j = 0;
            for (int i = 0; i < (n*(n-1)); i++)
                if (i%n != k)
                {
                    dataNew[j] = data[i];
                    j++;
                }
            float det1 = det*data[n*(n - 1) + k];
            determinant(det1, n - 1, dataNew, deter);
            delete dataNew;
            if ((n + k + 1) % 2 == 1)
                det = -(det);
        }
        return;
    }
    float minors(int I, int J, float Determinant) //if n = m
    {
        int count = 0;
        float* dataNew = new float [(n-1)*(n-1) + 1];
        for(int i=0; i<n*n; i++)
        {
            if(i%n == J || i/n == I)
            {}
            else
            {
                dataNew[count] = this->data[i] / Determinant;
                count++;
            }
        }
        float det = 1, dt = 0;
        determinant(det,n-1,dataNew, &dt);
        delete dataNew;
        return dt;
    }
public:
    MyMatrix()
    {
        this->n=0;
        this->m=0;
        this->data = NULL;
    }
    MyMatrix(int m, int n)
    {
        this->n = n;
        this->m = m;
        this->data = createNumbers(m,n);
    }
    virtual ~MyMatrix()
    {
        delete this->data;
    }
    virtual AbstractMatrix& operator+(AbstractMatrix& t)
    {
        if(this->n != t.getN() || this->m != t.getM() || failed() || t.failed())
        {
            MyMatrix* sum = new MyMatrix;
            return *sum;
        }
        MyMatrix* sum = new MyMatrix(this->m, this->n);
        for(int i=0; i<m; i++)
            for(int j=0; j<n; j++)
                sum->set(i,j,get(i,j)-t.get(i,j));
        return *sum;
    }
    virtual AbstractMatrix& operator = (AbstractMatrix& t)
    {
        n = t.getN();
        m = t.getM();
        delete data;
        data = new float[n*m];
        for (int i = 0; i < m; i++)
            for(int j = 0; j < n; j++)
                data[i*n+j] = t.get(i,j);
        return *this;
    }
    virtual AbstractMatrix& operator*(AbstractMatrix& t)
    {
        if(this->n != t.getM() || failed() || t.failed())
        {
            MyMatrix* mult = new MyMatrix;
            return *mult;
        }
        MyMatrix* mult = new MyMatrix(this->m, t.getN());
        for(int m = 0; m < mult->getM(); m++)
            for(int k = 0; k < mult->getN(); k++)
            {
                float sumMult = 0;
                for(int j=0; j<n; j++)
                    sumMult += (get(m,j) * t.get(j,k));
                mult->set(m,k,sumMult);
            }
        return *mult;
    }
    virtual AbstractMatrix& operator*(float& f)
    {
        if( failed() )
        {
            MyMatrix* mult = new MyMatrix;
            return *mult;
        }
        MyMatrix* mult = new MyMatrix(m,n);
        for(int i=0; i<m*n; i++)
            mult->data[i] = data[i] * f;
        return *mult;
    }
    virtual AbstractMatrix& operator-(AbstractMatrix& t)
    {
        if(this->n != t.getN() || this->m != t.getM() || failed() || t.failed())
        {
            MyMatrix* sum = new MyMatrix;
            return *sum;
        }
        MyMatrix* sum = new MyMatrix(this->m, this->n);
        for(int i=0; i<m; i++)
            for(int j=0; j<n; j++)
                sum->set(i,j,get(i,j)-t.get(i,j));
        return *sum;
    }
    virtual AbstractMatrix& reverse()
    {
        float Determinant = determinant();
        if( this->n != this->m || failed() || Determinant == 0)
        {
            return *this;
        }
        float* dataNew = new float [m*n];
        for(int i=0; i<m; i++)
            for(int j=0; j<n; j++)
                dataNew[i*m+j] = minors(i, j, Determinant);
        delete data;
        data = dataNew;
        return *this;
    }
    virtual AbstractMatrix& transpose()
    {
        float* nData = new float [n*m];
        for(int i=0; i<m; i++)
            for(int j=0; j<n; j++)
                nData[i*n+j] = get(j,i);
        delete data;
        data = nData;
        return *this;
    }
    virtual float determinant()
    {
        if( this->n != this->m || failed())
        {
            return 0;
        }
        float D = 0;
        determinant(1, n, data, &D);
        return D;
    }
    virtual ostream& print(ostream& o)
    {
        for(int i=0; i<m*n; i++)
        {
            if(i%n == 0)
                o << endl;
            o << data[i] << '\t';
        }
        o << endl;
        return o;
    }
    virtual istream& read(istream& o)
    {
        o >> this->m >> this->n;
        delete data;
        data = new float [m*n];
        for(int i=0; i<m*n; i++)
            o >> data[i];
        return o;
    }
    virtual float get (int i, int j)
    {
        int pos = getRightPos(i, j, getN());
        return this->data[pos];
    }
    virtual void set(int i, int j, float data)
    {
        int pos = getRightPos(i, j, getN());
        this->data[pos] = data;
    }
    virtual int getN ()
    {
        return this->n;
    }
    virtual int getM ()
    {
        return this->m;
    }
    virtual bool failed () //it will return true if matrix is fail
    {
        return (this->data == NULL);
    }
};

AbstractMatrix* get_init()
{
    MyMatrix* n = new MyMatrix(1,1);
    return n;
}
/*
int main()
{
    MyMatrix n(2,2);
    for(int i=0 ;i < n.getM(); i++)
        for(int j = 0; j < n.getN(); j++)
            n.set(i,j,1);
    MyMatrix m(3,3);
    m.set(0,0,0);
    m.set(0,1,0);
    m.set(0,2,1);
    m.set(1,0,0);
    m.set(1,1,1);
    m.set(1,2,0);
    m.set(2,0,1);
    m.set(2,1,0);
    m.set(2,2,0);
    m.print(cout);
    m.reverse();
    m.print(cout);
    return 0;
}
*/