#pragma once

#include <memory>
#include <map>
#include <list>

#include "../platformlayer/PlatformLayer.h"

namespace IMUST
{
    namespace ValueProxyType
    {
        const OJInt32_t Bool = 1;
        const OJInt32_t Char = 2;
        const OJInt32_t Int32 = 3;
        const OJInt32_t UInt32 = 4;
        const OJInt32_t Float16 = 5;
        const OJInt32_t Float32 = 6;
        const OJInt32_t String = 7;
        const OJInt32_t UserType = 0xff;
    }

    ///�ɱ����ӵ�ֵ
    class IValueProxy
    {
    public:
        IValueProxy();
        virtual ~IValueProxy();

        virtual OJInt32_t getType() const = 0;
        virtual bool isEqual(const IValueProxy * v) const = 0;

        virtual bool asBool() const { return false; }
        virtual OJChar_t asChar() const { return OJStr('\0'); }
        virtual OJInt32_t asInt32() const { return 0; }
        virtual OJUInt32_t asUInt32() const { return 0; }
        virtual OJFloat16_t asFloat16() const { return 0.0f;}
        virtual OJFloat32_t asFloat32() const { return 0.0;}
        virtual OJString asString() const { return EmptyString; }
    };

    typedef std::shared_ptr<IValueProxy> ValueProxyPtr;

    ValueProxyPtr buildWatchValue(const bool & v);
    ValueProxyPtr buildWatchValue(const OJChar_t & v);
    ValueProxyPtr buildWatchValue(const OJInt32_t & v);
    ValueProxyPtr buildWatchValue(const OJUInt32_t & v);
    ValueProxyPtr buildWatchValue(const OJFloat16_t & v);
    ValueProxyPtr buildWatchValue(const OJFloat32_t & v);
    ValueProxyPtr buildWatchValue(const OJString & v);
    ValueProxyPtr buildWatchValue(const OJChar_t *& v);

    class IWatchListener
    {
    public:
        IWatchListener();
        virtual ~IWatchListener();

        virtual void listen(ValueProxyPtr v) = 0;
    };

    ///��������һ���������������������Ӽ�������
    class Watch
    {
    public:
        typedef std::map<OJString, Watch*> WatchMap;

        ///���캯����name�в��ɺ�����'/'��
        explicit Watch(const OJString & name);
        ~Watch();

        ///��ȡһ���Ӽ�����
        Watch * getChild(const OJString & name, bool createIfMiss = true);

        ///�ݹ�Ļ�ȡһ���Ӽ�������name�ĸ�ʽ������a/b/c
        Watch * getWatch(const OJString & name, bool createIfMiss = true);

        ///ɾ��һҶ�ڵ��������name�ĸ�ʽ������a/b/c����ʱc�ᱻɾ����
        void delWatch(const OJString & name);

        ///���ӵ�����
        template<typename T>
        void watch(const OJString & name, const T & v)
        {
            Watch * w = getWatch(name);
            w->update( buildWatchValue(v) );
        }

        void update(ValueProxyPtr v);

        void addListener(IWatchListener * listener);
        void delListener(IWatchListener * listener);

        ValueProxyPtr getValue() const { return value_;}
        const OJString & getName() const { return name_; }

        WatchMap::iterator begin(){ return children_.begin(); }
        WatchMap::iterator end(){ return children_.end(); }

    protected:

        ///֪ͨ���е�listener
        void doNotify();

        ///��ֹ��������
        Watch(const Watch &);
        const Watch & operator=(const Watch &);


        OJString name_;         ///<������������
        ValueProxyPtr value_;   ///<�����ӵ�ֵ
        std::list<IWatchListener*> listeners_;///<�������ӵ�ֵ�����仯ʱ����֪ͨ��ÿ��listener
        WatchMap children_; ///<�Ӽ�����
        
    };


    Watch * rootWatch();

    void doWatchTest();
}