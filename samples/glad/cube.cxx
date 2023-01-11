/* Ⓒ 2023 - Sonejght */
#define GLFW_INCLUDE_NONE
#include <son8/opengl.hxx>
#include <GLFW/glfw3.h>

#include <iostream>
#include <cassert>

void data_create();
void data_accept();
void data_render();
void data_reject();
void data_delete();

void init_globals();

enum class DisplayMode : GLuint
{
    List,
    Draw,
    VBOm,
    VAOm,
};

enum class ProgramMode : GLuint
{
    None,
    S120,
    S330,
};

enum class ProfileMode : GLuint
{
    None,
    Comp = 0xCB,
    Core = 0xCE,
};

struct model
{
    inline static constexpr auto VERTICES = 8;
    inline static constexpr auto INDICES = 14;
    struct vertex {
        GLfloat pos[3];
        GLfloat col[3];
    };
    vertex vertices[VERTICES];
    GLuint indices[INDICES];
};

struct Globals {
    inline static constexpr auto VBO_SIZE = 2;
    inline static constexpr auto VAO_SIZE = 1;
    inline static constexpr auto ARRAY_BUFFER_INDEX = 0;
    inline static constexpr auto ELEMENT_BUFFER_INDEX = 1;
    using list_type = decltype(glGenLists(0));
    static constexpr model cube = {
        {
            { {+1,+1,+1}, {1, 1, 0} },
            { {-1,+1,+1}, {0, 1, 0} },
            { {+1,+1,-1}, {1, 1, 1} },
            { {-1,+1,-1}, {0, 1, 1} },
            { {+1,-1,+1}, {1, 0, 0} },
            { {-1,-1,+1}, {0, 0, 0} },
            { {-1,-1,-1}, {0, 0, 1} },
            { {+1,-1,-1}, {1, 0, 1} },
        },
        {3,2,6,7,4,2,0,3,1,6,5,4,1,0},
    };
    list_type list;
    GLuint vbo[VBO_SIZE];
    GLuint vao[VAO_SIZE];
    GLfloat rotateX;
    GLfloat rotateY;
    DisplayMode displayMode;
    ProgramMode programMode;
    ProfileMode profileMode;
    bool refreshFlag;
    bool firstrunFlag;
} *G;

int main()
{
    assert(glfwInit() == GLFW_TRUE);
    glfwSetErrorCallback([](auto error, auto msg) {
        std::cerr << error << ' ' << msg << '\n';
    });

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);

    auto window = glfwCreateWindow(640, 480, "glad-cube", nullptr, nullptr);
    assert(window != nullptr);
    glfwMakeContextCurrent(window);
    gladLoadGL((GLADloadfunc)glfwGetProcAddress);

    glfwSetKeyCallback(window, [](auto window, auto key, auto scancode, auto action, auto mods) {
        if (key ==GLFW_KEY_ESCAPE && action == GLFW_PRESS) glfwSetWindowShouldClose(window, GLFW_TRUE);
        if (action == GLFW_PRESS) {
            switch (key) {
                case GLFW_KEY_ESCAPE: glfwSetWindowShouldClose(window, GLFW_TRUE); break;
                case GLFW_KEY_R: G->refreshFlag = true; break;
            }
        }

        if (action == GLFW_REPEAT || action == GLFW_PRESS) {
            switch (key) {
                case GLFW_KEY_LEFT: G->rotateY += 15; break;
                case GLFW_KEY_RIGHT: G->rotateY -= 15; break;
                case GLFW_KEY_DOWN: G->rotateX -= 15; break;
                case GLFW_KEY_UP: G->rotateX += 15; break;
                default: break;
            }
        }
    });

    glfwSwapInterval(2);
    init_globals();

    while (!glfwWindowShouldClose(window)) {
        if (G->refreshFlag) {
            G->refreshFlag = false;
            if (!G->firstrunFlag) {
                data_reject();
                data_delete();
            }
            data_create();
            data_accept();
            G->rotateX = 45.f;
            G->rotateY = 45.f;
        }
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(-1.0, 1.0,-1.0, 1.0,-1.0, 1.0);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glRotatef(G->rotateX, 1.f, 0.f, 0.f);
        glRotatef(G->rotateY, 0.f, 1.f, 0.f);
        glScalef(.5f, .5f, .5f);

        data_render();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
}

void init_globals()
{
    static Globals g;
    g.refreshFlag = true;
    g.profileMode = ProfileMode::None;
    g.programMode = ProgramMode::None;
    g.displayMode = DisplayMode::VBOm;
    G = &g;
    glEnable(GL_DEPTH_TEST);
    glClearColor(.2f, .2f, .2f, 0.f);
}

void data_create()
{
    if (G->displayMode == DisplayMode::List) {
        G->list = glGenLists(1);
        glNewList(G->list, GL_COMPILE);
        {
            glBegin(GL_TRIANGLE_STRIP);
            {
                for (auto i : G->cube.indices) {
                    glColor3fv(G->cube.vertices[i].col);
                    glVertex3fv(G->cube.vertices[i].pos);
                }
            }
            glEnd();
        }
        glEndList();
    } else if (G->displayMode == DisplayMode::VBOm) {
        glGenBuffers(Globals::VBO_SIZE, G->vbo);
        glBindBuffer(GL_ARRAY_BUFFER, G->vbo[Globals::ARRAY_BUFFER_INDEX]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(G->cube.vertices), G->cube.vertices, GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, G->vbo[Globals::ELEMENT_BUFFER_INDEX]);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(G->cube.indices), G->cube.indices, GL_STATIC_DRAW);
        data_reject();
    }

    assert(glGetError() == GL_NO_ERROR);
}

void data_accept()
{
    if (G->displayMode != DisplayMode::List) {
        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_COLOR_ARRAY);

        void *pos, *col;
        if (G->displayMode == DisplayMode::Draw) {
            pos = (void *)G->cube.vertices;
            col = (GLubyte*)G->cube.vertices + offsetof(model::vertex, col);
        } else {
            glBindBuffer(GL_ARRAY_BUFFER, G->vbo[Globals::ARRAY_BUFFER_INDEX]);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, G->vbo[Globals::ELEMENT_BUFFER_INDEX]);
            pos = (void *)0;
            col = (void *)offsetof(model::vertex, col);
        }
        glVertexPointer(3, GL_FLOAT, sizeof(model::vertex), pos);
        glColorPointer(3, GL_FLOAT, sizeof(model::vertex), col);
    }
    assert(glGetError() == GL_NO_ERROR);
}

void data_render()
{
    data_accept();
    switch (G->displayMode) {
        case DisplayMode::List: glCallList(G->list); break;
        case DisplayMode::Draw: glDrawElements(GL_TRIANGLE_STRIP, model::INDICES, GL_UNSIGNED_INT, G->cube.indices); break;
        case DisplayMode::VBOm: glDrawElements(GL_TRIANGLE_STRIP, model::INDICES, GL_UNSIGNED_INT, (void *)0); break;
    }
    data_reject();
    assert(glGetError() == GL_NO_ERROR);
}

void data_reject()
{
    if (G->displayMode != DisplayMode::List) {
        if (G->displayMode == DisplayMode::VBOm) {
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        }

        glDisableClientState(GL_VERTEX_ARRAY);
        glDisableClientState(GL_COLOR_ARRAY);
    }
    assert(glGetError() == GL_NO_ERROR);
}

void data_delete()
{
    if (G->displayMode == DisplayMode::List) {
        glDeleteLists(G->list, 1);
    } else if (G->displayMode == DisplayMode::VBOm) {
        glDeleteBuffers(Globals::VBO_SIZE, G->vbo);
    }

    assert(glGetError() == GL_NO_ERROR);
}
