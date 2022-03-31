#if defined _bytereader_included_
  #endinput
#endif
#define _bytereader_included_

#if !defined BYTEREADER_BUFFERMAX
  #error Plugins must define BYTEREADER_MAX size prior to using this include
#endif

/**
 * An enum struct which provides a API to extract bytes from an underlying char buffer
 * 
 * @author JoinedSenses
 */
enum struct ByteReader {
	char data[BYTEREADER_BUFFERMAX];
	int size;
	int offset;

	void SetData(const char[] data, int dataSize, int startingOffset = 0) {
		if (dataSize > BYTEREADER_BUFFERMAX) {
			ThrowError(
				"ByteReader dataSize %i exceeds max size of %i. Increase BYTEREADER_BUFFERMAX if needed.",
				dataSize,
				BYTEREADER_BUFFERMAX
			);
		}

		if (startingOffset < 0 || startingOffset >= dataSize) {
			ThrowError(
				"ByteReader invalid starting offset %i. Must be in range of [0, %i)",
				startingOffset,
				dataSize
			);
		}

		for (int i = 0; i < dataSize; ++i) {
			this.data[i] = data[i];
		}
		this.data[dataSize] = 0;
		this.size = dataSize;
		this.offset = startingOffset;
	}

	int GetByte() {
		int offset = this.offset;
		if (offset < 0 || offset >= this.size) {
			ThrowError("Read offset %i out of bounds [0, %i)", offset, this.size);
		}

		int byte = this.data[offset];
		this.offset = offset + 1;
		return byte;
	}

	int GetShort() {
		return this.GetByte() | (this.GetByte() << 8);
	}

	int GetLong() {
		return this.GetShort() | (this.GetShort() << 16);
	}

	float GetFloat() {
		return view_as<float>(this.GetLong());
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

	bool GetString(char[] str, int size) {
		if (size < 0) { size = 0; }

		bool found = false;

		int j = 0;
		for (int i = this.offset; i < this.size; ++i, ++j) {
			if (j < size) {
				str[j] = this.data[i];
			}

			if (this.data[i] == '\x0') {
				found = true;
				break;
			}
		}

		this.offset += j + 1;

		if (size) { // Add null terminator
			int last = size - 1;
			str[j < last ? j + 1 : last] = 0;
		}

		return found;
	}

	int Remaining() {
		return this.size - this.offset;
	}
}