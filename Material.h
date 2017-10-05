#ifndef Material_h
#define Material_h


GLuint *texture = new GLuint[100];
int texture_index = 0;

class Material {
private:
public:
    int textureID;
    void LoadTextureFromFile(const char * filename) {
        textureID = texture_index;
        texture_index++;
        char header[54];
        int dataPos;
        int width,height;
        int imageSize;
        char *data;
        
        FILE *file = fopen(filename, "rb");
        
        if (!file) {
            printf("Image could not be opened\n");
            return;
        }
        if (fread(header,1,54,file) != 54) {
            printf("File must be BMP");
        }
        if (header[0] != 'B' || header[1] != 'M') {
            printf("File must be BMP");
            return;
        }
        dataPos = *(int*)&(header[0x0A]);
        imageSize = *(int*)&(header[0x22]);
        width = *(int*)&(header[0x12]);
        height = *(int*)&(header[0x16]);
        
        if (imageSize == 0)    imageSize = width*height * 3; // 3 : one byte for each Red, Green and Blue component
        if (dataPos == 0)      dataPos = 54; // seek to 1 bit after header
        
        // Create a buffer
        data = new char[imageSize];
        
        // Read the actual data from the file into the buffer
        fread(data, 1, imageSize, file);
        
        fclose(file);
        
        // Create one OpenGL texture
        glGenTextures(1, &texture[textureID]);
        
        // "Bind" the newly created texture : all future texture functions will modify this texture
        glBindTexture(GL_TEXTURE_2D, texture[textureID]);
        
        // Give the image to OpenGL
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);
        
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glBindTexture(GL_TEXTURE_2D, texture[0]);
    }
};


#endif