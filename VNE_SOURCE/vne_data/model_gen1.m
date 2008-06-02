% generate a shape for import to VNE
% create it as a levelset f(x,y,z) = 0

[xx,yy,zz] = meshgrid(-5:.125:5,-1:.125:1,-1:.125:1);

fxyz = 0.05*xx.^2 + 2*yy.^2 + 2*zz.^2 - 1.0;

% get faces and vertices
[f,v] = isosurface(xx,yy,zz,fxyz,0.0); 
[nf,nv] = reducepatch(f,v,20);
nv = nv/2;
n = isonormals(xx,yy,zz,fxyz,nv)

trisurf(nf,nv(:,1),nv(:,2),nv(:,3));

 csvwrite('faces3.dat',nf);
 csvwrite('verts3.dat',nv);
 csvwrite('norms3.dat',n);