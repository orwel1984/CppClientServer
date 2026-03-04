#pragma once

#include <filesystem>
#include <fstream>
#include <optional>
#include <vector>

class VideoFileReader
{
public:
    VideoFileReader(const std::filesystem::path &path) : m_file(path, std::ios::binary)
    {
        if (!m_file.is_open())
        {
            throw std::runtime_error("Failed to open file: " + path.string());
        }
        m_fileSize = std::filesystem::file_size(path);
    }

    ~VideoFileReader()
    {
        if (m_file.is_open())
        {
            m_file.close();
        }
    }

    std::optional<std::vector<std::byte>> 
    getNextVideoFrame()
    {
        // Read 4-byte length
        uint32_t frameLength;
        std::streampos headerPos = m_file.tellg();
        if (!m_file.read(reinterpret_cast<char *>(&frameLength), 4))
        {
            return {};
        }
        
        // Convert from big-endian to host endianness
        uint32_t hostFrameLength = __builtin_bswap32(frameLength);

        // // Read frame data without 4-byte Header
        // std::vector<std::byte> frame(frameLength);
        // if (!m_file.read(reinterpret_cast<char*>(frame.data()), frameLength))
        // {
        //     return { };
        // }

        // Seek back to the start of the header
        m_file.seekg(headerPos);

        // Read header + frame data
        std::vector<std::byte> frame(4 + hostFrameLength);
        if (!m_file.read(reinterpret_cast<char*>(frame.data()), 4 + hostFrameLength))
        {
            return {};
        }        

        return frame;
    }

    bool is_open() const { return m_file.is_open(); }
    int getFileSize() { return m_fileSize; }

private:
    std::ifstream m_file;
    int m_fileSize;
};
