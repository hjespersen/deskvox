// Shader for 2-channel data sets and simple alpha blending

// Authors of cg-version: 
//  Alexander Rice <acrice@cs.brown.edu>
//  Jurgen Schulze <schulze@cs.brown.edu>
//
// Converted by:
//   Stavros Delisavas <stavros.delisavas@uni-koeln.de>

uniform sampler3D pix3dtex;
uniform sampler2D pixLUT;
uniform vec4 opWeights;

void main()
{
  vec4 origColor = texture3D(pix3dtex, gl_TexCoord[0].xyz);
  vec4 OUT;

  OUT.x = texture2D(pixLUT, vec2(origColor.x, 0.0)).x;
  OUT.y = texture2D(pixLUT, vec2(origColor.y, 0.0)).y;
  OUT.z = 0.0;

  if (OUT.x > OUT.y)
    OUT.w = OUT.x * opWeights.x;
  else
    OUT.w = OUT.y * opWeights.y;

  gl_FragColor = OUT;
}
