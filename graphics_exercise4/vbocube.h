#ifndef VBOCUBE_H
#define VBOCUBE_H

class VBOCube
{
private:
    unsigned int vaoHandle;

public:
    VBOCube();

    void render();
};

#endif // VBOCUBE_H

struct scene{
	VBOCube *cube;
};

scene sc;
void startup()
{
	sc.cube = new VBOCube();

}

void render()
{

	sc.cube->render();
}