#include <vector>
#include <iostream>
#include <iomanip>
#include <random>
#include <bitset>

class chk {
	protected:
		size_t pos;
		std::vector<uint8_t> state;
		/* CHANGE THIS ITS JUST THE FLAG BECAUSE I'M STUPID */
		//std::vector<uint8_t> mod = {23, 238, 239, 18, 33, 32, 29, 2, 29, 10, 1, 44, 2, 9, 123, 164, 219, 128, 33, 253, 40, 28, 64, 74, 88, 83, 102, 100, 19, 16, 70, 62, 38, 7, 159, 129, 69, 42, 66, 73, 29};
		std::vector<uint8_t> mod = {70, 47, 37, 52 , 1, 54, 92, 20, 49, 91, 71, 87, 83, 18, 36, 30, 6, 68, 15, 58, 86, 29 , 4, 84, 2, 100, 61, 12, 97, 98, 13, 69, 63, 14, 80, 94, 44, 74, 2, 46, 78, 77, 73, 3, 26, 38, 60, 31, 62, 33, 89, 17, 8, 95, 90, 22, 93, 75, 81, 19, 57, 39, 27, 32, 85, 42, 7, 11};
		//std::vector<uint8_t> valid = {0, 26, 99, 116, 102, 123, 104, 101, 121, 95, 110, 111, 119, 95, 117, 95, 104, 97, 99, 107, 101, 100, 95, 109, 121, 95, 97, 108, 108, 115, 116, 97, 114, 125};
		std::vector<uint8_t> valid = {58, 24, 29, 64, 7, 69, 12, 81, 41, 94, 39, 26, 92, 77, 91, 58, 14, 29, 77, 32, 84, 69, 92, 9, 115, 51, 48, 93, 104, 17, 55, 86, 36, 79};
	public:
		chk(std::string input) {
			for (char c : input) {
				this->state.push_back(uint8_t(c));
			}
		}
		chk(std::vector<uint8_t> state) : state(state) {};
		virtual bool digicheck() { return false; }
		virtual uint8_t transform(uint8_t b) { return b; }
		virtual std::vector<uint8_t> decode() { return std::vector<uint8_t>(); }
		virtual ~chk() {}
		void fail() {
			std::cout << "You lose :)" << std::endl;
		}
};

class c_b : public chk {
	private:
		uint8_t cmp;
	public:
		c_b(std::vector<uint8_t> state, size_t pos, uint8_t cmp) : chk(state) {
			this->valid  = decode();
			this->state = state;
			this->pos = pos;
			this->cmp = cmp;
			if (this->pos % 2 == 0) {
				this->state[this->pos] = (this->state[this->pos] << (this->cmp % 8)) | (this->state[this->pos] >> (8 - (this->cmp % 8)));
				if (this->cmp != 0) {
					this->state[this->pos] ^= (this->cmp | this->pos);
				}
			} else {
				this->state[this->pos] = (this->state[this->pos] >> (this->cmp % 8)) | (this->state[this->pos] << (8 - (this->cmp % 8)));
				if (this->cmp != 0) {
					this->state[this->pos] ^= (this->cmp & this->pos);
				}
			}
			if (this->pos < this->state.size() && this->digicheck()) {
				this->pos++;
				this->cmp = this->state[this->pos];
				c_b(this->state, this->pos, this->cmp);
			} else if (!this->digicheck()) {
				this->fail();
			} else {
				std::cout << "Success!" << std::endl;
			}

			//printf("%d->%d\n", this->state[this->pos], this->cmp);
			//std::cout << std::bitset<8>(this->cmp) << std::endl;
			/*
			if (this->pos < this->state.size() && this->digicheck()) {
				c_b(this->state, this->pos, this->cmp);
			} else {
				this->fail();
			}
			*/
		}
		bool digicheck() {
			//printf("b-%d-%d\n", this->cmp, this->valid[this->pos]);
			if (this->cmp  == this->valid[this->pos]) {
				return true;
			}
			return false;
		}
		std::vector<uint8_t> decode() {
			std::vector<uint8_t> s = std::vector<uint8_t>(this->valid);
			std::vector<uint8_t> m = std::vector<uint8_t>(this->mod);
			std::srand(3542 ^ std::accumulate(m.begin(), m.end(), 0));	
			for (unsigned i = 0; i < m.size(); i++) {
                uint8_t tmp;
                size_t ind_a = std::rand() % m.size();
                size_t ind_b = std::rand() % m.size();
                tmp = m[ind_a];
                m[ind_a] = m[ind_b];
                m[ind_b] = tmp;
        	}
			for (unsigned i = 0; i < s.size(); i++) {
				s[i] += m[i];
			}
			return s;
		}
};

class c_a : public chk {
	private:
		uint8_t cmp;
	public:
		c_a(std::string input) : chk(input) {
			this->cmp = 26;
			this->pos = 0;
			if (this->digicheck()) {
				this->pos++;
				c_b(this->state, this->pos, this->cmp);
			} else {
				this->fail();
			}
		}
		uint8_t transform(uint8_t b) {
			b &= 0b00000010;
			b |= 0b01100000;
			//std::cout << std::bitset<8>(b) << std::endl;
			return b;
		}
		bool digicheck() {
			//printf("a-%d-%d\n", this->state[this->pos], this->transform(this->cmp));
			if (this->state[this->pos] == this->transform(this->cmp)) {
				return true;
			}
			return false;
		}

};

int main() {
	std::string in;
	std::cin >> in;
	c_a start(in);
	return 0;
}
