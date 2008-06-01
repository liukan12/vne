
% make up a postive definite symmetric matrix
mtrx = [6 2 3;
        2 5 1;
        3 1 9];
eigs(mtrx)

RHS = [1; 2; 3];

xhat = RHS; % initial guess

for i = 1:10
    prev = xhat;
    xhat(1) = ( RHS(1) - mtrx(2)*xhat(2) - mtrx(3)*xhat(3) )/mtrx(1);
    xhat(2) = ( RHS(2) - mtrx(4)*xhat(1) - mtrx(6)*xhat(3) )/mtrx(5);
    xhat(3) = ( RHS(3) - mtrx(7)*xhat(1) - mtrx(8)*xhat(2) )/mtrx(9);
    norm( xhat - prev )
    xhat
end;