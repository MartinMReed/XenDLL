#include <d3d8.h>
#include <d3dx8.h>

#pragma comment (lib, "d3dx8.lib")

typedef struct _D3DTLVERTEX {
    float sx; /* Screen coordinates */
    float sy;
    float sz;
    float rhw; /* Reciprocal of homogeneous w */
    D3DCOLOR color; /* Vertex color */
    float tu; /* Texture coordinates */
    float tv;
    _D3DTLVERTEX() { }
    _D3DTLVERTEX(const D3DVECTOR& v, float _rhw,
                 D3DCOLOR _color, 
                 float _tu, float _tv)
    { sx = v.x; sy = v.y; sz = v.z; rhw = _rhw;
      color = _color; 
      tu = _tu; tv = _tv;
    }
} D3DTLVERTEX, *LPD3DTLVERTEX;
 
#define D3DFVF_TLVERTEX D3DFVF_XYZRHW|D3DFVF_DIFFUSE|D3DFVF_TEX1

void BlitRect(LPDIRECT3DDEVICE8 lpDevice,
			  LPDIRECT3DTEXTURE8 lpSrc,
              RECT* destRect,
			  D3DXVECTOR4* srcTextureCoodRect,
              D3DCOLOR col,float z)
{
    // calculate rhw
	static float inc = 0.05f;
    static float rhw=1.0f; ///(z*990.0f+10.0f);
	if ((rhw<=0) || (rhw>=1))
		inc = - inc;

	rhw += inc;
	
    // set up rectangle
    D3DTLVERTEX verts[4];

    verts[0]=D3DTLVERTEX(D3DXVECTOR3(destRect->left-0.5f,  destRect->top-0.5f,    z),rhw,col,srcTextureCoodRect->x,srcTextureCoodRect->y); 
    verts[1]=D3DTLVERTEX(D3DXVECTOR3(destRect->right-0.5f, destRect->top-0.5f,    z),rhw,col,srcTextureCoodRect->z,srcTextureCoodRect->y);
    verts[2]=D3DTLVERTEX(D3DXVECTOR3(destRect->right-0.5f, destRect->bottom-0.5f,  z),rhw,col,srcTextureCoodRect->z,srcTextureCoodRect->w); 
    verts[3]=D3DTLVERTEX(D3DXVECTOR3(destRect->left-0.5f,  destRect->bottom-0.5f, z),rhw,col,srcTextureCoodRect->x,srcTextureCoodRect->w);

    // set the texture
    lpDevice->SetTexture(0,lpSrc);

    // configure shader for vertex type
    lpDevice->SetVertexShader(D3DFVF_TLVERTEX);

    // draw the rectangle

	//lpDevice->SetTextureStageState(0, D3DTSS_MINFILTER , D3DTEXF_POINT);

    lpDevice->DrawPrimitiveUP(D3DPT_TRIANGLEFAN,2,verts,sizeof(D3DTLVERTEX));
}
