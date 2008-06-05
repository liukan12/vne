% generate a shape for import to VNE
% create it as a levelset f(x,y,z) = 0

[xx,yy,zz] = meshgrid(-5:.125:5,-2:.125:2,-2:.125:2);

fxyz = xx.^20 + yy.^20 + zz.^20 - 1;

% get faces and vertices
[f,v] = isosurface(xx,yy,zz,fxyz,0.0); 
[nf,nv] = reducepatch(f,v,4);
nv = nv/2;
n = isonormals(xx,yy,zz,fxyz,nv);
trisurf(nf,nv(:,1),nv(:,2),nv(:,3));

 csvwrite('faces5.dat',nf);
 csvwrite('verts5.dat',nv);
 csvwrite('norms5.dat',n);