#pragma once
template<class C,bool FloderAsItem=false>
class FolderHelper
{
public:
	enum EFloderNodeType
	{
		FNT_Root,
		FNT_Floder,
		FNT_Object
	};
	struct Node
	{
		Node() :Object(nullptr), Type(FNT_Root), Selected(false){}
		~Node() {  }
		bool Selected;
		EFloderNodeType Type;
		shared_str Name;
		xr_vector<Node> Nodes;
		mutable C* Object;
		IC bool IsObject() { return Type == FNT_Object; }
		IC bool IsFloder() { return Type == FNT_Floder || Type == FNT_Root; }
	};
	FolderHelper() {}
	inline ~FolderHelper() {}
	inline Node* SelectObject(Node* N, const char* path)
	{
		VERIFY(N);
		VERIFY(N->IsFloder());
		if (strchr(path, '\\'))
		{
			string_path name;
			xr_strcpy(name, path);
			strchr(name, '\\')[0] = 0;
			N = FindFloder(N, name);
			VERIFY(N);
			N->Selected = true;
			return SelectObject(N, strchr(path, '\\') + 1);
		}
		for (Node& node : N->Nodes)
		{
			if (node.Name == path && node.IsObject())
			{
				node.Selected = true;
				return &node;
			}
		}
		VERIFY(false);
		return nullptr;
	}
	inline Node* FindObject(Node* N, const char* path)
	{
		if (N == nullptr) return nullptr;
		if (!N->IsFloder())
			return nullptr;
		if (strchr(path, '\\'))
		{
			string_path name;
			xr_strcpy(name, path);
			strrchr(name, '\\')[0] = 0;
			path = strrchr(path, '\\') + 1;
			N = FindFloder(N, name);
			if (N == nullptr) return nullptr;
		}
		for (Node& node : N->Nodes)
		{
			if (node.Name == path && node.IsObject())
				return &node;
		}
		return nullptr;
	}
	inline Node* FindFloder(Node* N, const char* path)
	{

		if (N == nullptr) return nullptr;
		if (!N->IsFloder())
			return nullptr;
		if (strchr(path, '\\'))
		{
			string_path name;
			xr_strcpy(name, path);
			strchr(name, '\\')[0] = 0;
			return AppendFloder(FindFloder(N, name), strchr(path, '\\') + 1);
		}
		for (Node& node : N->Nodes)
		{
			if (node.Name == path && node.IsFloder())
				return &node;
		}
		return nullptr;
	}
	inline Node* AppendFloder(Node* N, const char* path)
	{
		if (N == nullptr)return nullptr;
		if (strchr(path, '\\'))
		{
			string_path name;
			xr_strcpy(name, path);
			strchr(name, '\\')[0] = 0;
			{
				Node* NextNode = FindFloder(N, name);
				if (NextNode)
				{
					return AppendFloder(NextNode, strchr(path, '\\') + 1);
				}
			}
			return AppendFloder(AppendFloder(N, name), strchr(path, '\\') + 1);

		}

		for (Node& node : N->Nodes)
		{
			if (node.Name == path)
			{
				if constexpr (FloderAsItem)
				{
					node.Type = FNT_Floder;
					return &node;
				}
				else
				{
					if (node.IsFloder())
						return &node;
					return nullptr;
				}
			}
		}
		Node NewNode;
		NewNode.Type = FNT_Floder;
		NewNode.Name = path;
		N->Nodes.push_back(NewNode);
		return &N->Nodes.back();
	}
	inline Node* AppendObject(Node* N, const char* path)
	{
		if (N == nullptr)return nullptr;
		if (strchr(path, '\\'))
		{
			string_path name;
			xr_strcpy(name, path);
			strchr(name, '\\')[0] = 0;
			{
				Node* NextNode = FindFloder(N, name);
				if (NextNode)
				{
					return AppendObject(NextNode, strchr(path, '\\') + 1);
				}
			}
			return AppendObject(AppendFloder(N, name), strchr(path, '\\') + 1);

		}
		for (Node& node : N->Nodes)
		{
			if (node.Name == path)
			{
				if constexpr (FloderAsItem)
				{
					if (node.IsObject())
						return &node;
					return nullptr;
				}
				else
				{
					return &node;
				}
			}
		}
		Node NewNode;
		NewNode.Type = FNT_Object;
		NewNode.Name = path;
		N->Nodes.push_back(NewNode);
		return &N->Nodes.back();
	}
	inline void DrawNode(Node* N)
	{
		if (N->Type == FNT_Root)
		{
			for (Node& node : N->Nodes)
			{
				if (node.IsFloder()&& IsDrawFloder(&node))
				{
					DrawNode(&node);
				}
			}
			for (Node& node : N->Nodes)
			{
				if (!node.IsFloder())
				{
					DrawNode(&node);
				}
			}
		}
		else if (N->IsFloder())
		{
			if (N->Selected)ImGui::SetNextTreeNodeOpen(true);
			ImGui::AlignTextToFramePadding();
			if (ImGui::TreeNodeEx(N->Name.c_str(), IsFloderBullet(N)?ImGuiTreeNodeFlags_Bullet| ImGuiTreeNodeFlags_OpenOnArrow: ImGuiTreeNodeFlags_OpenOnArrow))
			{
				DrawAfterFloderNode();
				if (ImGui::IsItemClicked()&&N->Object)
					IsItemClicked(N);
				for (Node& node : N->Nodes)
				{
					if (node.IsFloder() && IsDrawFloder(&node))
					{
						DrawNode(&node);
					}
				}
				for (Node& node : N->Nodes)
				{
					if (!node.IsFloder())
					{
						DrawNode(&node);
					}
				}
				ImGui::TreePop();
			}
			else
			{
				DrawAfterFloderNode();
			}
			if (N->Selected)N->Selected = false;
		}
		else if (N->IsObject())
		{
			DrawItem(N);
			if (N->Selected)N->Selected = false;
		}
	}
	virtual void DrawAfterFloderNode() {}
	virtual bool IsFloderBullet(Node* Node) { return false; }
	virtual void IsItemClicked(Node* Node) {}
	virtual bool IsDrawFloder(Node* Node) = 0;
	virtual void DrawItem(Node* Node) = 0;
};
