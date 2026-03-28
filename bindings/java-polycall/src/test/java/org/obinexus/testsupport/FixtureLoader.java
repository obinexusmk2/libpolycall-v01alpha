package org.obinexus.testsupport;

import com.fasterxml.jackson.databind.ObjectMapper;
import org.yaml.snakeyaml.Yaml;

import java.io.IOException;
import java.io.InputStream;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.Map;

public final class FixtureLoader {
    private static final ObjectMapper JSON = new ObjectMapper();

    private FixtureLoader() {
    }

    public static Map<String, Object> load(String relativeFixturePath) throws IOException {
        Path repoRoot = resolveRepoRoot();
        Path fixturePath = repoRoot.resolve(relativeFixturePath).normalize();

        if (!Files.exists(fixturePath)) {
            throw new IOException("Fixture not found: " + fixturePath);
        }

        if (relativeFixturePath.endsWith(".json")) {
            return JSON.readValue(fixturePath.toFile(), Map.class);
        }

        if (relativeFixturePath.endsWith(".yaml") || relativeFixturePath.endsWith(".yml")) {
            try (InputStream is = Files.newInputStream(fixturePath)) {
                Yaml yaml = new Yaml();
                return yaml.load(is);
            }
        }

        throw new IOException("Unsupported fixture extension: " + relativeFixturePath);
    }

    private static Path resolveRepoRoot() {
        Path current = Paths.get("").toAbsolutePath();
        while (current != null) {
            if (Files.exists(current.resolve("fixtures")) && Files.exists(current.resolve("README.md"))) {
                return current;
            }
            current = current.getParent();
        }
        throw new IllegalStateException("Unable to resolve repository root from working directory");
    }
}
