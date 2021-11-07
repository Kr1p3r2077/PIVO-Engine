#pragma once

//using namespace std;

template <typename T = int>
class Mector {
private:
	T* arr;
	T default_value;
	int size = 0;
public:
	Mector(int size = 0, T place_holder = 0) {
		this->size = size;
		this->arr = new T[size];
		this->default_value = place_holder;
		for (int i = 0;i < size;i++) {
			this->arr[i] = place_holder;
		}
	}

	T& operator[](const int index) {
		if (index < this->size) {
			return this->arr[index];
		}
	}

	int getSize() {
		return this->size;
	}

	void Resize(int newSize = 0) {
		this->size = newSize;
	}

	void Print(bool endline = false) {
		for (int i = 0;i < this->size;i++) {
			cout << this->arr[i] << " ";
		}
		if (endline) { cout << endl; }
	}

	void Insert(int position, T object) {
		T* _arr = new T[this->size + 1];
		for (int i = 0;i < position;i++) {
			_arr[i] = this->arr[i];
		}

		_arr[position] = object;

		for (int i = position;i < this->size;i++) {
			_arr[i + 1] = this->arr[i];
		}

		this->arr = new T[this->size + 1];
		for (int i = 0;i < this->size + 1;i++) {
			this->arr[i] = _arr[i];
		}

		delete(_arr);
		this->size++;
	}

	void Add_end(T object) {
		Insert(this->size, object);
	}

	void Add_start(T object) {
		Insert(0, object);
	}

	void Del(int position) {
		this->size--;
		for (int i = position;i < this->size;i++) {
			this->arr[i] = arr[i + 1];
		}
	}

	void Del_end() {
		Del(this->size - 1);
	}

	void Del_start() {
		Del(0);
	}

	void Del_end_count(int k) {
		if (k <= 0) { return; }
		for (int i = 0;i < k;i++) {
			Del_end();
		}
	}

	void Del_start_count(int k) {
		if (k <= 0) { return; }
		for (int i = 0;i < k;i++) {
			Del_start();
		}
	}

	void Del_fromto(int k, int n) {

		if (k > n) {
			int tmp = k;
			k = n;
			n = tmp;
		}

		k--;
		int range = n - k;

		for (int i = 0;i < range;i++) {
			Del(k);
		}
	}

	void Reverse() {
		T* _arr = new T[this->size];

		for (int i = 0;i < this->size;i++) {
			_arr[i] = this->arr[i];
		}

		for (int i = 0;i < this->size;i++) {
			this->arr[i] = _arr[this->size - 1 - i];
		}

		delete(_arr);
	}

	void Reverse_fromto(int k, int n) {
		T* _arr = new T[this->size];

		for (int i = 0;i < this->size;i++) {
			_arr[i] = this->arr[i];
		}

		for (int i = k;i <= n;i++) {
			this->arr[i] = _arr[n - (i - k)];
		}

		delete(_arr);
	}

	void Sort_asc() {
		for (int i = 0;i < this->size - 1;i++) {
			if (this->arr[i] > this->arr[i + 1]) {
				T tmp = this->arr[i];
				this->arr[i] = this->arr[i + 1];
				this->arr[i + 1] = tmp;
			}
		}
	}

	void Sort_desc() {
		for (int i = 0;i < this->size - 1;i++) {
			if (this->arr[i] > this->arr[i + 1]) {
				T tmp = this->arr[i];
				this->arr[i] = this->arr[i + 1];
				this->arr[i + 1] = tmp;
			}
		}

		Reverse();
	}

	void SetDefaultValue(T object) {
		default_value = object;
	}

	void Clear() {
		for (int i = 0;i < this->size;i++) {
			this->arr[i] = default_value;
		}
	}

	/// <summary>
	/// beta
	/// </summary>
	/// <param name="m"></param>
	void Append(Mector m) {
		int oldsz = this->size;
		this->size += m.getSize();

		for (int i = oldsz;i < this->size;i++) {
			this->arr[i] = m[i - m.getSize()];
		}
	}

	int FindCountOf(T object) {
		int count = 0;

		for (int i = 0;i < this->size;i++) {
			if (this->arr[i] == object) {
				count++;
			}
		}

		return count;
	}
};