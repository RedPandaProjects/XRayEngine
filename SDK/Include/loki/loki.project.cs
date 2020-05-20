using BearBuildTool.Projects;
using System.IO;
using System;
public class loki :Project
{ 
	public loki(string ProjectPath)
	{
		Include.Public.Add(Path.Combine(ProjectPath,"include"));
	}
} 