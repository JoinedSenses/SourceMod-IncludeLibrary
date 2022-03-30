#if defined _bytereader_included_
  #endinput
#endif
#define _bytereader_included_

#define BYTEREADER_MAX 1024

/**
 * An enum struct which provides a API to extract bytes from an underlying char buffer
 * 
 * @author JoinedSenses
 */
enum struct ByteReader {
	char data[BYTEREADER_MAX];
	int size;
	int offset;

	void SetData(const char[] data, int dataSize, int offset) {
		for (int i = 0; i < dataSize; ++i) {
			this.data[i] = data[i];
		}
		this.data[dataSize] = 0;
		this.size = dataSize;
		this.offset = offset;
	}

	int GetByte() {
		return this.data[this.offset++];
	}

	int GetShort() {
		int x[2];
		x[0] = this.GetByte();
		x[1] = this.GetByte();
		return x[0] | x[1] << 8;
	}

	int GetLong() {
		int x[2];
		x[0] = this.GetShort();
		x[1] = this.GetShort();
		return x[0] | x[1] << 16;
	}

	void GetLongLong(char[] value, int size) {
		int x[2];
		x[0] = this.GetLong();
		x[1] = this.GetLong();

		KeyValues kv = new KeyValues("");
		kv.SetUInt64("value", x);
		kv.GetString("value", value, size);
		delete kv;
	}

	void GetString(char[] str, int size) {
		int j = 0;
		for (int i = this.offset; i < this.size; ++i, ++j) {
			if (j < size) {
				str[j] = this.data[i];
			}

			if (this.data[i] == '\x0') {
				break;
			}
		}

		this.offset += j + 1;
	}
}