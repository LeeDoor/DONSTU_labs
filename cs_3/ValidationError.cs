using System;

namespace ValidationLab;

public sealed class ValidationError
{
    public string Path { get; }
    public string Message { get; }
    public Attribute? Attribute { get; }

    public ValidationError(string path, string message, Attribute? attribute)
    {
        Path = path;
        Message = message;
        Attribute = attribute;
    }

    public override string ToString()
    {
        return $"{Path}: {Message}";
    }
}
