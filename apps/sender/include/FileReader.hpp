#pragma once

#include <filesystem>
#include <fstream>
#include <optional>
#include <vector>

template <std::size_t ChunkSize = 1024>
class FileReader
{
public:
    FileReader(const std::filesystem::path &path) : m_file(path, std::ios::binary)
    {
        if (!m_file.is_open())
        {
            throw std::runtime_error("Failed to open file: " + path.string());
        }
    }

    ~FileReader()
    {
        if (m_file.is_open())
        {
            m_file.close();
        }
    }

    std::optional<std::vector<char>> get_next_chunk()
    {
        if (!m_file || m_file.eof())
        {
            return {};
        }

        m_file.read(m_buffer.data(), ChunkSize);
        std::streamsize bytes_read = m_file.gcount();
        if (bytes_read <= 0)
        {
            return {};
        }

        return std::vector<char>(m_buffer.data(), m_buffer.data() + bytes_read);
    }

    bool is_open() const { return m_file.is_open(); }

private:
    std::ifstream m_file;
    std::array<char, ChunkSize> m_buffer;
};
