function [ ] = setGlobalpreviousPath ( val )
%SETGLOBALPREVIOUSPATH Sets global variable "previousPath".
%   SETGLOBALPREVIOUSPATH(val) just sets and save the value of "previousPath" global variable to "val".

global previousPath;
previousPath = val;
save previousPath.mat;