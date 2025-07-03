#pragma once

#include <filesystem>
#include <fstream>
#include <system_error>
#include <vector>

template <std::size_t ChunkSize = 1024>
class FileWriter
{
public:
    FileWriter(const std::filesystem::path& path) : m_file(path, std::ios::binary)
    {
        if (!m_file.is_open())
        {
            throw std::runtime_error("Failed to open file: " + path.string());
        }
    }

    ~FileWriter()
    {
        if (m_file.is_open())
        {
            m_file.close();
        }
    }

    std::error_code writeChunk(const std::vector<char>& chunk)
    {
        m_file.write(chunk.data(), chunk.size());
        if (!m_file)
        {
            return std::make_error_code(std::errc::io_error);
        }
        return {};
    }

    std::error_code writeChunk(std::string_view chunk)
    {
        m_file.write(chunk.data(), chunk.size());
        if (!m_file)
        {
            return std::make_error_code(std::errc::io_error);
        }
        return {};
    }

    bool is_open() const { return m_file.is_open(); }

private:
    std::ofstream m_file;
};
