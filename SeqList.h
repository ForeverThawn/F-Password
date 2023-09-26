#include <iostream>

typedef int OperationReturnErrLevel;

template <typename ElemType> class SeqList {
public:
    enum PushAndPopPosition {
        back = false,
        front = true
    };

    enum ErrCode {
        PositionPointerOutOfRange = -2,
        ObjectListNotFound = -1,
        DataNotFoundInObject = -3,

        OperationOk = 0,
        OperationErr = 1
    };

private:
    struct SqList {
        ElemType* data;
        size_t length;
        size_t maxSize;
    } sq;

public:
    SeqList() {
        sq.data = nullptr;
        sq.length = 0;
        new_capacity(4);
        sq.maxSize = 4;
    }

    SeqList(size_t max_reserve_size) {
        sq.data = nullptr;
        sq.length = 0;
        new_capacity(max_reserve_size);
        sq.maxSize = max_reserve_size;
    }

    ~SeqList() {
        free(sq.data);
        sq.data = nullptr;
        sq.length = 0;
        sq.maxSize = 0;
    }

    size_t capacity() {
        return sq.maxSize;
    }

    OperationReturnErrLevel __stdcall push(ElemType data, bool position = back) {
        if (position == front)
            return insert(data, 0);
        return insert(data, sq.length);
    }

    OperationReturnErrLevel __stdcall pop(bool position = back) {
        if (position == front)
            return erase(0);
        return erase(sq.length);
    }

    OperationReturnErrLevel __stdcall pop(ElemType& data, bool position = back) {
        if (position == front)
            return erase(0, &data);
        return erase(sq.length, &data);
    }

    bool __stdcall find(ElemType data) {
        for (auto i = 0; i < sq.length; i++)
            if (sq.data[i] == data)
                return i;
        return DataNotFoundInObject;
    }

    bool __stdcall exists(ElemType data) {
        for (auto i = 0; i < sq.length; i++)
            if (sq.data[i] == data)
                return true;
        return false;
    }

    OperationReturnErrLevel __stdcall insert(ElemType data, size_t position) {
        if (!sq.data)
            return ObjectListNotFound;
        if (!(0 <= position && position <= sq.maxSize))
            return PositionPointerOutOfRange;

        if (sq.length == sq.maxSize)  // 提前加容量
            double_capacity();

        for (auto p = sq.maxSize; p > position; p--)  // 开位置
            sq.data[p] = sq.data[p - 1];

        sq.data[position] = data;  // 插
        sq.length++;
        return OperationOk;
    }

    OperationReturnErrLevel __stdcall erase(size_t position) {
        if (!sq.data)
            return ObjectListNotFound;
        if (!(0 <= position && position <= sq.maxSize))
            return PositionPointerOutOfRange;

        for (auto p = position; p < sq.length; p++)   // 开位置
            sq.data[p] = sq.data[p + 1];

        sq.length--;
        return OperationOk;
    }

    OperationReturnErrLevel __stdcall erase(size_t position, ElemType* erased_data) {
        if (!sq.data)
            return ObjectListNotFound;
        if (!(0 <= position && position <= sq.maxSize))
            return PositionPointerOutOfRange;

        *erased_data = sq.data[position];
        for (auto p = position; p < sq.length; p++)   // 开位置
            sq.data[p] = sq.data[p + 1];

        sq.length--;
        return OperationOk;
    }

    size_t size() {
        return sq.length;
    }

    ElemType at(size_t position) {
        return sq.data[position];
    }

private:
    void double_capacity() {
        sq.data = (ElemType*)realloc(sq.data, sizeof(ElemType) * sq.maxSize * 2);
        sq.maxSize *= 2;
    }

    void new_capacity(size_t size) {
        sq.data = (ElemType*)malloc(sizeof(ElemType) * size);
        sq.maxSize = size;
    }

};