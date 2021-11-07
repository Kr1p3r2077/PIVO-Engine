#pragma once
#define TEXTURE_H
#include "stb_image.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <iostream>
#include <thread>
class Texture {
public:
    int width, height, nrChannels;
    unsigned int ID;
    unsigned char* data;

    void Import(std::string path) {
        data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);

        glGenTextures((int)1, &ID);
        glBindTexture(GL_TEXTURE_2D, ID);

        if (data) {

            std::cout << "Texture " << path << " has " << nrChannels << " channels.\n";

            if (nrChannels == 3) {
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
                glGenerateMipmap(GL_TEXTURE_2D);
            }
            if (nrChannels == 4) {
                if (std::to_string(path.find_last_of('.')) != ".hdr") {
                    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
                    glGenerateMipmap(GL_TEXTURE_2D);
                }
                else {
                    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, data);
                    glGenerateMipmap(GL_TEXTURE_2D);
                }
            }
            if (nrChannels == 2) {
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RG, width, height, 0, GL_RG, GL_UNSIGNED_BYTE, data);
                glGenerateMipmap(GL_TEXTURE_2D);
            }
            if (nrChannels == 1) {
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, data);
                glGenerateMipmap(GL_TEXTURE_2D);
            }
        }
        else {
            std::cout << "Failed to generate texture: " << path << "\n";
        }
        stbi_image_free(data);
    }

public:

    /*void Import(std::string path) {
        std::thread th(Import1,path);
    }*/

    Texture& operator= (const Texture& a) {
        width = a.width;
        height = a.height;
        nrChannels = a.nrChannels;
        ID = a.ID;
        data = a.data;
        return *this;
    }

    void Activate() {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, ID);
    }

    void Activate(int number) {

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glActiveTexture(GL_TEXTURE0 + number);
        glBindTexture(GL_TEXTURE_2D, ID);
    }
};